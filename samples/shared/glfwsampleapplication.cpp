/*
 * Copyright (c) 2014, Denis Biryukov <denis.birukov@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Denis Biryukov <denis.birukov@gmail.com> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Denis Biryukov <denis.birukov@gmail.com> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "glfwsampleapplication.h"
#include "GLFW/glfw3.h"
#include <iostream>

#include "libcppprofiler/src/cppprofiler.h"

GLFWwindow *GlfwSampleApplication::window = nullptr;
std::function<void(double, double, int, int)> GlfwSampleApplication::mMouseMoveCallback;
std::function<void(double, double, int, int, int)> GlfwSampleApplication::mMouseButtonCallback;
std::function<void(int, int, int, int)> GlfwSampleApplication::mKeyboardCallback;
std::function<void(unsigned int)> GlfwSampleApplication::mCharCallback;
std::function<void(double, double)> GlfwSampleApplication::mScrollCallback;

void glfwError(int error, const char *desc)
{
	PROFILE_FUNCTION

	std::cerr << "Error: " << desc << std::endl;
}

bool GlfwSampleApplication::init(int width, int height, std::function<void(void)> runFunction)
{
	PROFILE_FUNCTION

	if (!glfwInit())
	{
		std::cerr << "Unable to initialize glfw" << std::endl;
		return false;
	}

	glfwSetErrorCallback(glfwError);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(width, height, "Sample Application", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Unable to create glfw window." << std::endl;
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(window, mCursorPos);
	glfwSetMouseButtonCallback(window, mMouseButton);
	glfwSetKeyCallback(window, mKey);
	glfwSetCharCallback(window, mChar);
	glfwSetScrollCallback(window, mScroll())

	this->runFunction = runFunction;

	return true;
}

void GlfwSampleApplication::run()
{
	PROFILE_FUNCTION

	for (;;)
	{
		PROFILE_BLOCK(frame, "Frame");
		
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)return;
		if (glfwWindowShouldClose(window) == GL_TRUE)return;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
		this->runFunction();

		PROFILE_START("Swap buffers");
		glfwSwapBuffers(window);
		PROFILE_END
	}
}

void GlfwSampleApplication::renderCube()
{
	PROFILE_FUNCTION

	const GLfloat vertices[][3] = {{ -1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, { -1.0, 1.0, -1.0}, { -1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, { -1.0, 1.0, 1.0}};
	const GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
	static float angle_x = 0, angle_y = 0;

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(angle_x, 1.0, 0.0, 0.0);
	glRotatef(angle_y, 0.0, 1.0, 0.0);

#define DRAW_FACE(a,b,c,d) \
	glBegin(GL_POLYGON); \
	glColor3fv(colors[a]); \
	glVertex3fv(vertices[a]); \
	glColor3fv(colors[b]); \
	glVertex3fv(vertices[b]); \
	glColor3fv(colors[c]); \
	glVertex3fv(vertices[c]); \
	glColor3fv(colors[d]); \
	glVertex3fv(vertices[d]); \
	glEnd();

	DRAW_FACE(0, 3, 2, 1);
	DRAW_FACE(2, 3, 7, 6);
	DRAW_FACE(0, 4, 7, 3);
	DRAW_FACE(1, 2, 6, 5);
	DRAW_FACE(4, 5, 6, 7);
	DRAW_FACE(0, 1, 5, 4);

#undef DRAW_FACE
	angle_x += 1;
	angle_y += 1;

}

void GlfwSampleApplication::mChar(GLFWwindow *wnd, unsigned int c)
{
	if (mCharCallback) mCharCallback(c);
}

void GlfwSampleApplication::mCursorPos(GLFWwindow *wnd, double x, double y)
{
	if (mMouseMoveCallback)
	{
		int buttonMask = 0;
		for (int a = GLFW_MOUSE_BUTTON_1; a < GLFW_MOUSE_BUTTON_LAST; ++a)
		{
			if (glfwGetMouseButton(window, a) == GLFW_PRESS)
			{
				buttonMask |= 1 << a;
			}
		}

		int mods = 0;

		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			mods |= GLFW_MOD_ALT;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			mods |= GLFW_MOD_SHIFT;

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			mods |= GLFW_MOD_CONTROL;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS)
			mods |= GLFW_MOD_SUPER;

		mMouseMoveCallback(x, y, buttonMask, mods);
	}
}

void GlfwSampleApplication::mKey(GLFWwindow *wnd, int key, int scancode, int action, int mods)
{
	if (mKeyboardCallback)mKeyboardCallback(key, scancode, action, mods);
}

void GlfwSampleApplication::mMouseButton(GLFWwindow *wnd, int button, int action, int mods)
{
	if (mMouseButtonCallback)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mMouseButtonCallback(x, y, button, action, mods);
	}
}

void GlfwSampleApplication::setCharCallback(std::function<void(unsigned int)> callback)
{
	mCharCallback = callback;
}

void GlfwSampleApplication::setKeyboardCallback(std::function<void(int, int, int, int)> callback)
{
	mKeyboardCallback = callback;
}

void GlfwSampleApplication::setMouseButtonCallback(std::function<void(double, double, int, int, int)> callback)
{
	mMouseButtonCallback = callback;
}

void GlfwSampleApplication::setMouseMoveCallback(std::function<void(double, double, int, int)> callback)
{
	mMouseMoveCallback = callback;
}

void GlfwSampleApplication::mScroll(GLFWwindow *wnd, double x, double y)
{
	if (mScrollCallback) mScrollCallback(x, y);
}

void GlfwSampleApplication::setScrollCallback(std::function< void(double, double) > callback)
{
	mScrollCallback = callback;
}

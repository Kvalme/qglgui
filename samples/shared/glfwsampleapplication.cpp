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

void glfwError(int error, const char *desc)
{
	std::cerr << "Error: " << desc << std::endl;
}

bool GlfwSampleApplication::init(int width, int height, std::function<void(void)> runFunction)
{
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
	
	this->runFunction = runFunction;

	return true;
}

void GlfwSampleApplication::run()
{
	for (;;)
	{
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)return;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderCube();
		this->runFunction();

		glfwSwapBuffers(window);
	}
}

void GlfwSampleApplication::renderCube()
{
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


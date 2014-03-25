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

#ifndef GLFWSAMPLEAPPLICATION_H
#define GLFWSAMPLEAPPLICATION_H

#include "GLFW/glfw3.h"

#include <functional>

class GlfwSampleApplication
{
public:
	void run();
	bool init(int width, int height, std::function<void(void)> runFunction);
	void setMouseMoveCallback(std::function<void(double, double, int, int)> callback);
	void setMouseButtonCallback(std::function<void(double, double, int, int, int)> callback);
	void setKeyboardCallback(std::function<void(int, int, int, int)> callback);
	void setCharCallback(std::function<void(unsigned int)> callback);
	void setScrollCallback(static std::function<void(double, double)> callback);
	
private:
	void renderCube();
	static GLFWwindow *window;
	std::function<void(void)> runFunction;
	static std::function<void(double, double, int, int)> mMouseMoveCallback;
	static std::function<void(double, double, int, int, int)> mMouseButtonCallback;
	static std::function<void(int, int, int, int)> mKeyboardCallback;
	static std::function<void(unsigned int)> mCharCallback;
	static std::function<void(double, double)> mScrollCallback;
	
	static void mCursorPos(GLFWwindow *wnd, double x, double y);
	static void mMouseButton(GLFWwindow *wnd, int button, int action, int mods);
	static void mKey(GLFWwindow *wnd, int key, int scancode, int action, int mods);
	static void mChar(GLFWwindow *wnd, unsigned int c);
	static void mScroll(GLFWwindow *wnd, double x, double y);
	
};

#endif // GLFWSAMPLEAPPLICATION_H

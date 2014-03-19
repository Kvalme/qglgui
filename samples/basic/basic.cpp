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
#include "qglgui/glgui.h"
#include "qglgui/glguirenderer.h"
#include "basicqtapp.h"

#include <memory>
#include <iostream>

#include "libcppprofiler/src/cppprofiler.h"

using namespace QGL;

std::shared_ptr<GlGui> gui;

void mouseMove(double x, double y, int buttonState, int mods)
{
	gui->InjectMouseMoveEvent(0, QPoint(x, y), (buttonState & 1) == (1) ? Qt::LeftButton : Qt::NoButton, 0);
}

void mouseButton(double x, double y, int button, int action, int mods)
{
	gui->InjectMouseButtonEvent(0, QPoint(x, y), action == GLFW_PRESS ? Qt::LeftButton : Qt::NoButton, 0);
}

void keyboard(int key, int scancode, int action, int mods)
{
}

void character(unsigned int c)
{
}

QWidget *createWindow(const std::string &)
{
	PROFILE_FUNCTION

	return new BasicQtApp;
}

void run()
{
	PROFILE_FUNCTION

	gui->Update();
	gui->Render();
}

void initGui()
{
	PROFILE_FUNCTION

	gui = GlGui::Create(GlGui::THREADING_MODE::SINGLE, "../../../fonts", QRect(0, 0, 800, 600));
	gui->RegisterWindowFactory(createWindow);
	gui->RegisterRenderer(CreateRenderer(RENDERER_TYPE::GL1));
	gui->CreateWindow("");
}

int main()
{
	PROFILE_FUNCTION
	
	GlfwSampleApplication app;
	if (!app.init(800, 600, run))return 1;

	initGui();

	app.setCharCallback(character);
	app.setKeyboardCallback(keyboard);
	app.setMouseButtonCallback(mouseButton);
	app.setMouseMoveCallback(mouseMove);
	app.run();

	gui.reset();
	
	return 0;
}

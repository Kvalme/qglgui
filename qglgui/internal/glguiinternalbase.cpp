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

#include "qglgui/internal/glguiinternalbase.h"
#include "qtplugin/uiwindow.h"
#include "glguiwindowdecoratoraurorae.h"
#include <qglgui/glguirenderer.h>
#include "libcppprofiler/src/cppprofiler.h"

#include <assert.h>
#include <qpa/qwindowsysteminterface.h>
#include <QApplication>

using namespace QGL;

GlGuiInternalBase::GlGuiInternalBase(const std::string &fontDir) :
	mKeyboardGrabWindow(nullptr),
	mMouseGrabWindow(nullptr)
{
	PROFILE_FUNCTION
	mDecorator = new GlUIWindowDecoratorAurorae;
}

GlGuiInternalBase::~GlGuiInternalBase()
{
	PROFILE_FUNCTION

	windows.clear();
	delete guiRenderer;
	delete mDecorator;
}

void GlGuiInternalBase::RegisterWindowFactory(std::function<void(const std::string &name)> factory)
{
	PROFILE_FUNCTION

	windowFactory = factory;
}

void GlGuiInternalBase::RegisterRenderer(GlGuiRenderer *renderer)
{
	PROFILE_FUNCTION

	if (!renderer)return;

	guiRenderer = renderer;
}

void GlGuiInternalBase::iAddWindow(UIWindow *wnd)
{
	PROFILE_FUNCTION

	assert(wnd);
	assert(guiRenderer);

	windows.insert(std::make_pair(wnd->winId(), Window(wnd)));

	guiRenderer->GuiCreateWindow(wnd->winId(), wnd);
}

void GlGuiInternalBase::iRemoveWindow(unsigned int winId)
{
	PROFILE_FUNCTION

	assert(guiRenderer);

	windows.erase(winId);

	guiRenderer->GuiRemoveWindow(winId);
}

void GlGuiInternalBase::iSetTexture(unsigned int winId, const QPixmap &pixmap)
{
	PROFILE_FUNCTION

	assert(guiRenderer);

	auto it = windows.find(winId);
	if (it == windows.end())
		throw std::runtime_error("Setting texture for not-existed window is impossible");

	guiRenderer->GuiSetTexture(winId, &pixmap);
}

GlGuiInternalBase::Window::Window(UIWindow *w) :
	wnd(w)
{
	PROFILE_FUNCTION

}

void GlGuiInternalBase::InjectMouseButtonEvent(int screenId, QPoint position, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	QWindow *wnd = handleMouseEvent(screenId, position, button, modifiers);
/*	if (wnd && mActiveWindow && wnd != mActiveWindow)
	{
		mActiveWindow.
	}*/
	if (wnd)
	{
		mActiveWindow = wnd;
		mActiveWindow->requestActivate();
	}
	if (button != Qt::LeftButton) mMouseGrabWindow = nullptr;
}

void GlGuiInternalBase::InjectMouseMoveEvent(int screenId, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	handleMouseEvent(screenId, position, buttons, modifiers);
}

void GlGuiInternalBase::InjectCharacterEvent(QChar character)
{
	PROFILE_FUNCTION

	QWindowSystemInterface::handleKeyEvent(NULL, QEvent::KeyPress, Qt::Key::Key_A, 0, character);
}

void GlGuiInternalBase::InjectKeyboardEvent(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	QWindowSystemInterface::handleKeyEvent(NULL, eventType, key, modifiers);
}

QWindow *GlGuiInternalBase::handleMouseEvent(int screenId, QPoint position, Qt::MouseButtons b, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	QWindow *wnd = mMouseGrabWindow ? mMouseGrabWindow->window() : nullptr;
	if (!wnd)
	{
		Window *w = getWindowByPoint(position);
		wnd = w ? w->wnd->window() : nullptr;
	}

	if (!wnd) return nullptr;
	
	QPointF local = wnd->mapFromGlobal(position);
	QPoint l(local.x(), local.y());
	
	if (mDecorator->handleMouseEvent(wnd, l, position, Qt::MouseButtons(b), modifiers))
	{
		//Click was handled by decorator - no need to pass it to QT
		return wnd;
	}
	
	QWindowSystemInterface::handleMouseEvent(wnd, local, position, Qt::MouseButtons(b), modifiers);
	return wnd;
}

GlGuiInternalBase::Window *GlGuiInternalBase::getWindowByPoint(QPoint point)
{
	PROFILE_FUNCTION

	Window *wnd = nullptr;
	for (auto & w_pair : windows)
	{
		Window *tmp_wnd = &w_pair.second;
		if (!tmp_wnd->wnd) continue;
		if (!tmp_wnd->wnd->isVisible())continue;
		QRect geom = tmp_wnd->wnd->window()->frameGeometry();

		if (geom.contains(point))
		{
			if (wnd && tmp_wnd->wnd->getZLevel() > wnd->wnd->getZLevel()) wnd = tmp_wnd;
			else if (!wnd) wnd = tmp_wnd;
		}
	}

	return wnd;
}

void GlGuiInternalBase::iGrabKeyboard(UIWindow *wnd)
{
	PROFILE_FUNCTION

	mKeyboardGrabWindow = wnd;
}

void GlGuiInternalBase::iGrabMouse(UIWindow *wnd)
{
	PROFILE_FUNCTION

	mMouseGrabWindow = wnd;
}

void GlGuiInternalBase::InjectMouseWheelEvent(int screenId, QPoint position, double deltax, double deltay, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION
	
	QWindow *wnd = mMouseGrabWindow ? mMouseGrabWindow->window() : nullptr;
	if (!wnd)
	{
		Window *w = getWindowByPoint(position);
		wnd = w ? w->wnd->window() : nullptr;
	}
	if (!wnd) return;
	
	QPointF local = wnd->mapFromGlobal(position);
	
	QPoint angDelta(deltax, deltay);
	QPoint pxDelta(angDelta * 10);
	
	QWindowSystemInterface::handleWheelEvent(wnd, local, position, pxDelta, angDelta, modifiers);
}

bool GlGuiInternalBase::IsKeyboardGrabbed()
{
	return (mKeyboardGrabWindow != nullptr);
}

bool GlGuiInternalBase::IsMouseGrabbed()
{
	return (mMouseGrabWindow != nullptr);
}

bool GlGuiInternalBase::IsPointInsideWindow(QPoint point)
{
	return (getWindowByPoint(point) != nullptr);
}

bool GlGuiInternalBase::IsFocusWindow()
{
	return (QApplication::focusWidget() != nullptr);
}

void GlGuiInternalBase::RegisterWindowDecorator(GlUIWindowDecorator *decorator)
{
	delete mDecorator;
	mDecorator = decorator;
}

void GlGuiInternalBase::SetWindowTheme(const std::string &path, const std::string &name)
{
	if (mDecorator)
	{
		mDecorator->SetTheme(path, name);
		for(auto &w : windows)
		{
			QWindowSystemInterface::handleThemeChange(w.second.wnd->window());
		}
	}
}

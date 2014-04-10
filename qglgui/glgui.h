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

#pragma once
#include <memory>
#include <functional>
#include <QRect>
#include <QEvent>
#include <QChar>

class QWidget;

namespace QGL
{

class GlGuiRenderer;

/**
 * Main class for GlGui. Handles GUI initialization.
 * Can work in both - multi and single thread enviroment.
 */
class GlGui
{
public:
	enum class THREADING_MODE
	{
		SINGLE,
		MULTI
	};

	/**
	 * Create GlGui instance to operate in specified threading mode.
	 *
	 * @param mode threading mode
	 * @param fontDir path to font directory
	 * @param viewport viewport for primary screen GUI that will be used as screen size.
	 * @return pointer to created gui
	 */
	static std::shared_ptr<GlGui> Create(THREADING_MODE mode, const std::string &fontDir, QRect viewport);
	
	/**
	 * Creates QT screen.
	 * 
	 * @param viewport viewport of the newly created screen
	 * @return screen id. -1 in case of error.
	 */
	virtual int CreateScreen(QRect viewport) = 0;

	/**
	 * Registers window factory.
	 * Each call to CreateWindow will try to create window via provided factory
	 *
	 * @param factory pointer to the factory
	 */
	virtual void RegisterWindowFactory(std::function<void(const std::string &name)> factory) = 0;

	/**
	 * Register renderer.
	 *
	 * @param renderer pointer to renderer object
	 */
	virtual void RegisterRenderer(GlGuiRenderer *renderer) = 0;

	/**
	 * Creates window by provided name.
	 * GuiWindowFactory will be called to create window.
	 * Window isn't saved inside UI library. It should be managed by it self.
	 *
	 * @note Window is kept inside class implementation. Window->show() automatically called
	 *
	 * @param name window name
	 * @return window id or 0 in case of error
	 */
	virtual void CreateWindow(const std::string &name) = 0;
	
	/**
	 * Renders GUI using previously registered renderer.
	 * Should be called from application from render thread
	 */
	virtual void Render() = 0;

	/**
	 * Updates gui. Proceed internal events and so.
	 */
	virtual void Update() = 0;
	
	/**
	 * Injects mouse move event into QT event queue
	 * 
	 * @param screenId idientifyer of the screen on which event happened
	 * @param positon current mouse position
	 * @param buttons pressed mouse buttons
	 * @param modifiers active keyboard modifiers
	 */
	virtual void InjectMouseMoveEvent(int screenId, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) = 0;

	/**
	 * Injects mouse button event into QT event queue
	 * 
	 * @param screenId idientifyer of the creen on which event happened
	 * @param position current mouse position
	 * @param button pressed button
	 * @param modifiers active keyboard modifiers
	 */
	virtual void InjectMouseButtonEvent(int screenId, QPoint position, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
	
	/**
	 * Injects mouse wheel event into QT event queue
	 * 
	 * @param screenId identifyer of the screen on which event happened
	 * @param position current mouse position
	 * @param deltax offset of mouse wheel on X axis
	 * @param deltay offset of mousewheel on Y axis
	 * @param modifiers active keyboard modifiers
	 */
	virtual void InjectMouseWheelEvent(int screenId, QPoint position, double deltax, double deltay, Qt::KeyboardModifiers modifiers) = 0;
	
	/**
	 * Injects keyboard event into QT event queue
	 * Always passes key to current active window
	 * 
	 * @param eventType Qt::KeyPress or Qt::KeyRelease
	 * @param key key
	 * @param modifiers modifiers state (shift, meta, ctrl)
	 */
	virtual void InjectKeyboardEvent(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modifiers) = 0;
	
	/**
	 * Injects character event into QT event queue
	 * Always passes character to current active window
	 * 
	 * @param character character to inject
	 */
	virtual void InjectCharacterEvent(QChar character) = 0;
	
	/**
	 * Checks if point is inside any window
	 * @param point point
	 * @return true is point inside window
	 */
	virtual bool IsPointInsideWindow(QPoint point) = 0;
	
	/**
	 * Checks if mouse is grabbed or not
	 * @return true if mouse is grabbed
	 */
	virtual bool IsMouseGrabbed() = 0;
	
	/**
	 * Checks if keyboard is grabbed or not
	 * @return true if keyboard is grabbed
	 */
	virtual bool IsKeyboardGrabbed() = 0;
	
	/**
	 * Checks if any widnow has input focus
	 * @return true if some window has input focus
	 */
	virtual bool IsFocusWindow() = 0;
	
protected:
	GlGui();
	GlGui(GlGui &);
	virtual ~GlGui();
};

}


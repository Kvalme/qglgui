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

#include "qglgui/internal/glguimultithread.h"
#include "qglgui/internal/qtplugin/uiworker.h"
#include <qglgui/glguirenderer.h>

#include "libcppprofiler/src/cppprofiler.h"
#include <QWidget>
#include <assert.h>


using namespace QGL;

GlGuiMultiThread::GlGuiMultiThread(const std::string &fontDir, QRect viewport)
	: GlGuiInternalBase(fontDir)
{
	PROFILE_FUNCTION
	uiWorker = std::shared_ptr<UIWorker>(new UIWorker(this, fontDir, viewport));
	mGuiThreadId = std::this_thread::get_id();
}

GlGuiMultiThread::~GlGuiMultiThread()
{
	PROFILE_FUNCTION
}

void GlGuiMultiThread::CreateWindow(const std::string &name)
{
	PROFILE_FUNCTION
	
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);

		mTasks.push_back(std::bind(&GlGuiMultiThread::CreateWindow, this, name));
	}
	else
	{
		QWidget *w = windowFactory(name);
		assert(w);
		w->show();
	}
}

void GlGuiMultiThread::Render()
{
	PROFILE_FUNCTION
	
	assert(mGuiThreadId != std::this_thread::get_id());
	assert(guiRenderer);
	
	guiRenderer->Render();
}

void GlGuiMultiThread::Update()
{
	PROFILE_FUNCTION
	
	std::lock_guard<std::mutex> guard(mMainMutex);
	
	for (std::function<void(void)> &f : mTasks)
	{
		f();
	}
	mTasks.clear();
	
	uiWorker->Update();
}

int GlGuiMultiThread::CreateScreen(QRect viewport)
{
	PROFILE_FUNCTION
	
	return -1;
}

void GlGuiMultiThread::InjectCharacterEvent(QChar character)
{
	PROFILE_FUNCTION
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);
		mTasks.push_back(std::bind(&GlGuiMultiThread::InjectCharacterEvent, this, character));
	}
	else
	{
		QGL::GlGuiInternalBase::InjectCharacterEvent(character);
	}
}

void GlGuiMultiThread::InjectKeyboardEvent(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);
		mTasks.push_back(std::bind(&GlGuiMultiThread::InjectKeyboardEvent, this, eventType, key, modifiers));
	}
	else
	{
		QGL::GlGuiInternalBase::InjectKeyboardEvent(eventType, key, modifiers);
	}
}

void GlGuiMultiThread::InjectMouseButtonEvent(int screenId, QPoint position, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);
		mTasks.push_back(std::bind(&GlGuiMultiThread::InjectMouseButtonEvent, this, screenId, position, button, modifiers));
	}
	else
	{
		QGL::GlGuiInternalBase::InjectMouseButtonEvent(screenId, position, button, modifiers);
	}
}

void GlGuiMultiThread::InjectMouseMoveEvent(int screenId, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);
		mTasks.push_back(std::bind(&GlGuiMultiThread::InjectMouseMoveEvent, this, screenId, position, buttons, modifiers));
	}
	else
	{
		QGL::GlGuiInternalBase::InjectMouseMoveEvent(screenId, position, buttons, modifiers);
	}
}

void GlGuiMultiThread::InjectMouseWheelEvent(int screenId, QPoint position, double deltax, double deltay, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION
	if (mGuiThreadId != std::this_thread::get_id())
	{
		std::lock_guard<std::mutex> guard(mMainMutex);
		mTasks.push_back(std::bind(&GlGuiMultiThread::InjectMouseWheelEvent, this, screenId, position, deltax, deltay, modifiers));
	}
	else
	{
		GlGuiInternalBase::InjectMouseWheelEvent(screenId, position, deltax, deltay, modifiers);
	}
}


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

#include "qglgui/internal/glguisinglethread.h"
#include "qglgui/internal/qtplugin/uiworker.h"
#include "qglgui/glguirenderer.h"

#include "libcppprofiler/src/cppprofiler.h"
#include <QWidget>
#include <assert.h>

using namespace QGL;

GlGuiSingleThread::GlGuiSingleThread(const std::string &fontDir, QRect viewport)
	: GlGuiInternalBase(fontDir)
{
	PROFILE_FUNCTION

	uiWorker = std::shared_ptr<UIWorker>(new UIWorker(this, fontDir, viewport));
}

GlGuiSingleThread::~GlGuiSingleThread()
{
	PROFILE_FUNCTION

}

void GlGuiSingleThread::CreateWindow(const std::string &name)
{
	PROFILE_FUNCTION
	
	QWidget *w = windowFactory(name);
	assert(w);
	w->show();
}

void GlGuiSingleThread::Render()
{
	PROFILE_FUNCTION

	assert(guiRenderer);
	
	guiRenderer->Render();
}

void GlGuiSingleThread::Update()
{
	PROFILE_FUNCTION

	uiWorker->Update();
}

void GlGuiSingleThread::iSetTexture(unsigned int winId, QPixmap pixmap)
{
	PROFILE_FUNCTION
	
	QGL::GlGuiInternalBase::iSetTexture(winId, pixmap);
}

int GlGuiSingleThread::CreateScreen(QRect viewport)
{
	return uiWorker->CreateScreen(viewport);
}


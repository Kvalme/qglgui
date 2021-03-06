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

#include "uiworker.h"

#include <private/qguiapplication_p.h>
#include <QtWidgets/QApplication>
#include "QtGui/qpa/qplatformthemefactory_p.h"
#include "QtGui/qpa/qplatformtheme.h"
#include <QtPlugin>

#include "qglgui/internal/qtplugin/uiintegration.h"
#include "qglgui/internal/glguiinternalbase.h"

#include "libcppprofiler/src/cppprofiler.h"

using namespace QGL;

Q_IMPORT_PLUGIN(QGLIntegrationPlugin);

UIWorker::UIWorker(GlGuiInternalBase *gui, const std::string &fontDir, QRect viewport, float dpix, float dpiy)
{
	PROFILE_FUNCTION

	this->gui = gui;

	static int argc = 0;
	static char **argv = nullptr;

	qputenv("QT_QPA_FONTDIR", fontDir.c_str());
	qputenv("QT_QPA_PLATFORM", "QGL");

	QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, true);

	mPlatform = new UIIntegration(this->gui, viewport, dpix, dpiy);

	app = new QApplication(argc, argv);

	app->setQuitOnLastWindowClosed(false);
}

UIWorker::~UIWorker()
{
	PROFILE_FUNCTION

//	delete app;
}

void UIWorker::Update()
{
	PROFILE_FUNCTION
	std::lock_guard<std::mutex> guard(mEventMutex);
	
	app->processEvents();
}

int UIWorker::CreateScreen(QRect viewport, float dpix, float dpiy)
{
	PROFILE_FUNCTION

	return mPlatform->addScreen(viewport, dpix, dpiy);
}

void UIWorker::BeginEvent()
{
	PROFILE_FUNCTION

	mEventMutex.lock();
}

void UIWorker::EndEvent()
{
	PROFILE_FUNCTION

	mEventMutex.unlock();
}


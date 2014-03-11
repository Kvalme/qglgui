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

#include "uiintegration.h"
#include "libcppprofiler/src/cppprofiler.h"
#include "uiwindow.h"
#include "uiscreen.h"
#include "uibackingstore.h"

#if defined(Q_OS_UNIX)
#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>
#if defined(Q_OS_MAC)
#include <qpa/qplatformfontdatabase.h>
#else
#include <QtPlatformSupport/private/qgenericunixfontdatabase_p.h>
#endif
#elif defined(Q_OS_WIN)
#include <QtPlatformSupport/private/qbasicfontdatabase_p.h>
#include <QtCore/private/qeventdispatcher_win_p.h>
#endif

#include <private/qpixmap_raster_p.h>
#include <private/qguiapplication_p.h>

#include <qpa/qplatformservices.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>

#include "qglgui/internal/glguiinternalbase.h"

using namespace QGL;

UIIntegration::UIIntegration(GlGuiInternalBase *gui)
{
	PROFILE_FUNCTION

#if defined(Q_OS_UNIX)
	event_dispatcher = createUnixEventDispatcher();
#if defined(Q_OS_MAC)
	font_database = new QPlatformFontDatabase();
#else
	font_database = new QGenericUnixFontDatabase();
#endif
#elif defined(Q_OS_WIN)
	event_dispatcher = new QEventDispatcherWin32;
	font_database = new QBasicFontDatabase();
#endif

	services_ = new QPlatformServices;

	QCoreApplicationPrivate::eventDispatcher = event_dispatcher;
	ui = gui;
}

void UIIntegration::init(QRect viewport)
{
	PROFILE_FUNCTION

	QGuiApplicationPrivate::instance()->setEventDispatcher(event_dispatcher);
	addScreen(viewport);
}

int UIIntegration::addScreen(QRect viewport)
{
	screenAdded(new UIScreen(viewport));
	return mScreenId++;
}

UIIntegration::~UIIntegration()
{
	PROFILE_FUNCTION

	delete event_dispatcher;
	delete font_database;
	delete services_;
}

QPlatformBackingStore *UIIntegration::createPlatformBackingStore(QWindow *window) const
{
	PROFILE_FUNCTION

	return new UIBackingStore(window);
}

QPlatformWindow *UIIntegration::createPlatformWindow(QWindow *window) const
{
	PROFILE_FUNCTION

	UIWindow *wnd = new UIWindow(window);
	wnd->requestActivateWindow();

	ui->iAddWindow(wnd);
	return wnd;
}

QAbstractEventDispatcher *UIIntegration::guiThreadEventDispatcher() const
{
	PROFILE_FUNCTION

	return event_dispatcher;
}

QPlatformFontDatabase *UIIntegration::fontDatabase() const
{
	PROFILE_FUNCTION

	return font_database;
}

QPlatformServices *UIIntegration::services() const
{
	PROFILE_FUNCTION

	return services_;
}

bool UIIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
	PROFILE_FUNCTION

	switch (cap)
	{
		case ThreadedPixmaps:
			return true;
		case MultipleWindows:
			return true;
		case WindowMasks:
			return true;
		default:
			return QPlatformIntegration::hasCapability(cap);
	}
}

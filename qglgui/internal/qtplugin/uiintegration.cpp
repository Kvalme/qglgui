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
#include "uifontdatabase.h"

#if defined(Q_OS_UNIX)
#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>
#elif defined(Q_OS_WIN)
#include <QtCore/private/qeventdispatcher_win_p.h>
#endif

#include <private/qpixmap_raster_p.h>
#include <private/qguiapplication_p.h>

#include <qpa/qplatformservices.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>

#include "qglgui/internal/glguiinternalbase.h"

#include <qpa/qplatformintegrationplugin.h>

using namespace QGL;

UIIntegration *UIIntegration::instance = nullptr;

class QGLIntegrationPlugin : public QPlatformIntegrationPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "QGL.json")
public:
	QPlatformIntegration *create(const QString &, const QStringList &);
};

QPlatformIntegration *QGLIntegrationPlugin::create(const QString &system, const QStringList &paramList)
{
	if (!system.compare(QLatin1String("QGL"), Qt::CaseInsensitive))
	{
		UIIntegration::instance->init();
		return UIIntegration::instance;
	}

	return 0;
}

#include "uiintegration.moc"

UIIntegration::UIIntegration(GlGuiInternalBase *gui, QRect viewport, float dpix, float dpiy)
{
	PROFILE_FUNCTION

	font_database = new UIFontDatabase;
	services_ = new QPlatformServices;

//	QCoreApplicationPrivate::eventDispatcher = event_dispatcher;
	ui = gui;
	instance = this;
	mDefaultViewport = viewport;
	mDpix = dpix;
	mDpiy = dpiy;
}

QAbstractEventDispatcher *UIIntegration::createEventDispatcher() const
{
//	return event_dispatcher;
#if defined(Q_OS_UNIX)
	return createUnixEventDispatcher();
#elif defined(Q_OS_WIN)
	return new QEventDispatcherWin32;
#endif
}

void UIIntegration::init()
{
	PROFILE_FUNCTION

	addScreen(mDefaultViewport, mDpix, mDpiy);
}

int UIIntegration::addScreen(QRect viewport, float dpix, float dpiy)
{
	UIScreen *s = new UIScreen(viewport, dpix, dpiy);
	mScreens.push_back(s);
    QWindowSystemInterface::handleScreenAdded(s);
	return mScreenId++;
}

UIIntegration::~UIIntegration()
{
	PROFILE_FUNCTION

	for (UIScreen * s : mScreens)
    {
        QWindowSystemInterface::handleScreenRemoved(s);
        delete s;
    }

    //	delete event_dispatcher;
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

/*QAbstractEventDispatcher *UIIntegration::guiThreadEventDispatcher() const
{
	PROFILE_FUNCTION

	return event_dispatcher;
}*/

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

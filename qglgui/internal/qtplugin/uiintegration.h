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
#include <qpa/qplatformintegration.h>
#include <../qtdeclarative-opensource-src-5.2.1/src/quick/scenegraph/qsgcontextplugin_p.h>
#include <qplugin.h>

#include <thread>

namespace QGL
{

class UIOpenGLContext;

class GlGuiInternalBase;
class UIScreen;

class UIIntegration: public QPlatformIntegration
{
public:
	UIIntegration(QGL::GlGuiInternalBase *gui, QRect viewport, std::function<void(void)> makeOffscreenCurrent);
	~UIIntegration();

	void init();
	bool hasCapability(QPlatformIntegration::Capability cap) const;

	QPlatformWindow *createPlatformWindow(QWindow *window) const;
	QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const;
	QAbstractEventDispatcher *createEventDispatcher() const;
	QPlatformServices *services() const;
	
	QPlatformOpenGLContext *createPlatformOpenGLContext(QOpenGLContext *context) const;

	QPlatformFontDatabase *fontDatabase() const;
//	QAbstractEventDispatcher *guiThreadEventDispatcher() const;

	GlGuiInternalBase *getUi()
	{
		return ui;
	}

	int addScreen(QRect viewport);

	static UIIntegration *instance;
	
	static std::thread::id guiThreadId;
	
private:
	QAbstractEventDispatcher *event_dispatcher;
	QPlatformFontDatabase *font_database;
	QPlatformServices *services_;
	GlGuiInternalBase *ui;
	int mScreenId = 0;
	std::vector<UIScreen *> mScreens;
	QRect mDefaultViewport;
	UIOpenGLContext *mContext;
	std::function<void(void)> mMakeOffscreenCurrent;
};

class ContextPlugin : public QSGContextPlugin
{
	Q_OBJECT

	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QSGContextFactoryInterface" FILE "customcontext.json")

public:
	ContextPlugin(QObject *parent = 0);

	QStringList keys() const;
	QSGContext *create(const QString &key) const;
	QQuickTextureFactory *createTextureFactoryFromImage(const QImage &image);
};

}

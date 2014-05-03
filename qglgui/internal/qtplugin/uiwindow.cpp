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

#include "uiwindow.h"
#include "libcppprofiler/src/cppprofiler.h"
#include "uiintegration.h"
#include <qglgui/internal/glguiinternalbase.h>
#include <qglgui/glguiwindowdecorator.h>

#include <qpa/qplatformscreen.h>
#include <qpa/qwindowsysteminterface.h>

#include <private/qwindow_p.h>
#include <private/qguiapplication_p.h>


using namespace QGL;

std::map<WId, UIWindow *> UIWindow::windows_by_win_id;
UIWindow *UIWindow::top_level_window = nullptr;

UIWindow::UIWindow(QWindow *window) :
	QPlatformWindow(window),
	position_includes_frame(false),
	visible(false),
	pending_geometry_change_on_show(true),
	z_level(0.0),
	opacity_level(1.0)
{
	PROFILE_FUNCTION

	QRect geom(window->geometry());
	QScreen *screen = window->screen();
	QRect screenGeom(screen->availableGeometry());
	int x = screenGeom.x() + screenGeom.width()/2. - geom.width()/2.;
	int y = screenGeom.y() + screenGeom.height()/2. - geom.height()/2.;
	geom.setX(x);
	geom.setY(y);
	
	setGeometry(geom);
	setWindowFlags(window->flags());
	setWindowState(window->windowState());

	QWindowSystemInterface::flushWindowSystemEvents();

	static WId counter = 0;
	win_id = ++counter;

	raise();
	mIsDecorationUpdateNeeded = true;
	checkDecorations();
}

UIWindow::~UIWindow()
{
	PROFILE_FUNCTION

	GlGuiInternalBase *ui = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration)->getUi();
	ui->iRemoveWindow(win_id);

	if (top_level_window == this) top_level_window = nullptr;

	if (ui->iGetKeyboardGrab() == this)
	{
		ui->iGrabKeyboard(nullptr);
	}

	if (ui->iGetMouseGrab() == this)
	{
		ui->iGrabMouse(nullptr);
	}
}

void UIWindow::requestActivateWindow()
{
	PROFILE_FUNCTION

	if (visible)
	{
		if (!window()->isActive()) mIsDecorationUpdateNeeded = true;
		QWindowSystemInterface::handleWindowActivated(window());
		raise();
	}
}

void UIWindow::setFrameMarginsEnabled(bool enabled)
{
	PROFILE_FUNCTION

	if (enabled && !(window()->flags() & Qt::FramelessWindowHint))
		margins = QMargins(2, 2, 2, 2);
	else
		margins = QMargins(0, 0, 0, 0);
}

void UIWindow::setGeometry(const QRect &rect)
{
	PROFILE_FUNCTION

	if (window()->windowState() != Qt::WindowNoState) return;

	position_includes_frame = qt_window_private(window())->positionPolicy == QWindowPrivate::WindowFrameInclusive;

	setFrameMarginsEnabled(true);
	setGeometryImpl(rect);

	normal_geometry = geometry();
}

void UIWindow::setGeometryImpl(const QRect &rect)
{
	PROFILE_FUNCTION

	QRect adjusted = rect;
	if (adjusted.width() <= 0) adjusted.setWidth(1);
	if (adjusted.height() <= 0) adjusted.setHeight(1);

	if (position_includes_frame)
	{
		adjusted.translate(margins.left(), margins.top());
	}
	else
	{
		// make sure we're not placed off-screen
		if (adjusted.left() + adjusted.width() < margins.left()) adjusted.translate(margins.left(), 0);
		if (adjusted.top() + adjusted.height() < margins.top()) adjusted.translate(0, margins.top());
	}

	if (visible && (geometry().width() != adjusted.width() || geometry().height() != adjusted.height()))
	{
		mIsDecorationUpdateNeeded = true;
	}

	QPlatformWindow::setGeometry(adjusted);

	if (visible)
	{
		QWindowSystemInterface::handleGeometryChange(window(), adjusted);
		QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(), adjusted.size()));
	}
	else
	{
		pending_geometry_change_on_show = true;
	}
}

void UIWindow::setVisible(bool vis)
{
	PROFILE_FUNCTION
	if (visible == vis) return;

	if (vis)
	{
		mIsDecorationUpdateNeeded = true;

		if (window()->type() != Qt::ToolTip)
			QWindowSystemInterface::handleWindowActivated(window());

		if (pending_geometry_change_on_show)
		{
			pending_geometry_change_on_show = false;
			QWindowSystemInterface::handleGeometryChange(window(), geometry());
		}

		QRect rect(QPoint(), geometry().size());
		QWindowSystemInterface::handleExposeEvent(window(), rect);
		if (window()->isModal())
			z_level = 0.5;
	}
	else
	{
		QWindowSystemInterface::handleExposeEvent(window(), QRegion());
	}

	visible = vis;
}

void UIWindow::setWindowState(Qt::WindowState state)
{
	PROFILE_FUNCTION

	setFrameMarginsEnabled(state != Qt::WindowFullScreen);
	position_includes_frame = false;

	switch (state)
	{
		case Qt::WindowFullScreen:
			setGeometryImpl(screen()->geometry());
			break;
		case Qt::WindowMaximized:
			setGeometryImpl(screen()->availableGeometry().adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom()));
			break;
		case Qt::WindowMinimized:
			break;
		case Qt::WindowNoState:
			setGeometryImpl(normal_geometry);
			break;
		default:
			break;
	}
	
	checkDecorations();

	QWindowSystemInterface::handleWindowStateChanged(window(), state);
}

void UIWindow::setWindowFlags(Qt::WindowFlags flags)
{
	PROFILE_FUNCTION

	mIsDecorationUpdateNeeded = true;

	QPlatformWindow::setWindowFlags(flags);
}

void UIWindow::lower()
{
	PROFILE_FUNCTION

	z_level = 0.0;
	mIsDecorationUpdateNeeded = true;
}

void UIWindow::raise()
{
	PROFILE_FUNCTION

	if (top_level_window)
	{
		if (top_level_window->window()->isAncestorOf(window()))
		{
			top_level_window->z_level = 0.9;
		}
		else
		{
			top_level_window->lower();
		}
	}
	top_level_window = this;

	z_level = 1.0;
	mIsDecorationUpdateNeeded = true;
}

void UIWindow::setOpacity(qreal level)
{
	PROFILE_FUNCTION

	opacity_level = level;
}

bool UIWindow::setKeyboardGrabEnabled(bool grab)
{
	PROFILE_FUNCTION

	UIIntegration *i = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration);
	if (grab) i->getUi()->iGrabKeyboard(this);
	else i->getUi()->iGrabKeyboard(nullptr);
	return true;
}

bool UIWindow::setMouseGrabEnabled(bool grab)
{
	PROFILE_FUNCTION

	UIIntegration *i = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration);
	if (grab) i->getUi()->iGrabMouse(this);
	else i->getUi()->iGrabMouse(nullptr);
	return true;
}

void UIWindow::windowEvent(QEvent *event)
{
	PROFILE_FUNCTION
	
	QPlatformWindow::windowEvent(event);
}

bool UIWindow::startSystemResize(const QPoint &pos, Qt::Corner corner)
{
	PROFILE_FUNCTION

	mIsDecorationUpdateNeeded = true;

	setMouseGrabEnabled(true);

	return QPlatformWindow::startSystemResize(pos, corner);
}

void UIWindow::propagateSizeHints()
{
//	QPlatformWindow::propagateSizeHints();
}

QMargins UIWindow::frameMargins() const
{
	PROFILE_FUNCTION
	
	if (isDecorationsNeeded)
	{
		UIIntegration *i = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration);
		GlUIWindowDecorator *decorator = i->getUi()->getDecorator();
		if (decorator) return decorator->GetFrameMargins();
	}
	return QPlatformWindow::frameMargins();
}

void UIWindow::checkDecorations()
{
	PROFILE_FUNCTION
	
	isDecorationsNeeded = false;
	switch (window()->type())
	{
		case Qt::Window:
		case Qt::Widget:
		case Qt::Dialog:
		case Qt::Tool:
		case Qt::Drawer:
			isDecorationsNeeded = true;
			break;
		default:
			break;
	}
	if (window()->flags() & Qt::FramelessWindowHint || window()->windowState() == Qt::WindowFullScreen)
		isDecorationsNeeded = false;
	if (window()->flags() & Qt::BypassWindowManagerHint)
		isDecorationsNeeded = false;
	
	mIsDecorationUpdateNeeded = true;
}

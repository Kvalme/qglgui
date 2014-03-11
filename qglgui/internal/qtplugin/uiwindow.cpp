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

	if (window->windowState() == Qt::WindowNoState)
		setGeometry(window->geometry());
	else
		setWindowState(window->windowState());

	QWindowSystemInterface::flushWindowSystemEvents();

	static WId counter = 0;
	win_id = ++counter;

	raise();
}

UIWindow::~UIWindow()
{
	PROFILE_FUNCTION

	/*	MagicUI *ui = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration)->getMagicUI();
		ui->_removeWindow(_win_id_);

		if (_top_level_window_ == this) _top_level_window_ = nullptr;

		if (ui->_getKeyboardGrabWinwdow() == this)
		{
			ui->_setKeyboardGrabWindow(nullptr);
		}

		if (ui->_getMouseGrabWinwdow() == this)
		{
			ui->_setMouseGrabWindow(nullptr);
		}*/

}

void UIWindow::requestActivateWindow()
{
	PROFILE_FUNCTION

	if (visible)
	{
		QWindowSystemInterface::handleWindowActivated(window());
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
	QWindowSystemInterface::handleWindowStateChanged(window(), state);
}

void UIWindow::setWindowFlags(Qt::WindowFlags flags)
{
	PROFILE_FUNCTION

	QPlatformWindow::setWindowFlags(flags);
}

void UIWindow::lower()
{
	PROFILE_FUNCTION

	z_level = 0.0;
}

void UIWindow::raise()
{
	PROFILE_FUNCTION

	if (top_level_window)
	{
		top_level_window->lower();
	}
	top_level_window = this;

	z_level = 1.0;
}

void UIWindow::setOpacity(qreal level)
{
	PROFILE_FUNCTION

	opacity_level = level;
}

bool UIWindow::setKeyboardGrabEnabled(bool grab)
{
	PROFILE_FUNCTION

//	UIIntegration *i = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration);
	/*	if (grab) i->getMagicUI()->_setKeyboardGrabWindow(this);
		else i->getMagicUI()->_setKeyboardGrabWindow(nullptr);*/
	return true;
}

bool UIWindow::setMouseGrabEnabled(bool grab)
{
	PROFILE_FUNCTION

//	UIIntegration *i = static_cast<UIIntegration *>(QGuiApplicationPrivate::platform_integration);
	/*	if (grab) i->getMagicUI()->_setMouseGrabWindow(this);
		else i->getMagicUI()->_setMouseGrabWindow(nullptr);*/
	return true;
}

void UIWindow::windowEvent(QEvent *event)
{
	PROFILE_FUNCTION

}

bool UIWindow::startSystemResize(const QPoint &pos, Qt::Corner corner)
{
	PROFILE_FUNCTION

	setMouseGrabEnabled(true);

	return QPlatformWindow::startSystemResize(pos, corner);
}

void UIWindow::propagateSizeHints()
{
//	QPlatformWindow::propagateSizeHints();
}



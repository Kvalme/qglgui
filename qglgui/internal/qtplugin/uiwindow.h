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

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>

#include <map>

namespace QGL
{
class UIWindow: public QPlatformWindow
{
public:
	UIWindow(QWindow *window);
	~UIWindow();

	void setGeometry(const QRect &rect);
	void setWindowState(Qt::WindowState state);
	void setWindowFlags(Qt::WindowFlags flags);

	QMargins frameMargins() const;

	void setVisible(bool visible);
	bool isVisible()
	{
		return visible;
	}
	void requestActivateWindow();
	bool setKeyboardGrabEnabled(bool grab);
	bool setMouseGrabEnabled(bool grab);
	void windowEvent(QEvent *event);

	WId winId() const
	{
		return win_id;
	}

	void raise();
	void lower();

	void setOpacity(qreal level);
	float getOpacity() const
	{
		return opacity_level;
	}

	static UIWindow *windowByWinId(WId id);

	float getZLevel() const
	{
		return z_level;
	}

	bool startSystemResize(const QPoint &pos, Qt::Corner corner);

	void propagateSizeHints();
	
	bool IsDecorated() const { return isDecorationsNeeded;}
	
	bool IsDecorationUpdateNeeded() const { return mIsDecorationUpdateNeeded;}
	void SetDecorationUpdate(bool state = false) { mIsDecorationUpdateNeeded = state;}


private:
	void setFrameMarginsEnabled(bool enabled);
	void setGeometryImpl(const QRect &rect);
	void checkDecorations();

	QRect normal_geometry;
	QMargins margins;
	bool position_includes_frame;
	bool visible;
	bool pending_geometry_change_on_show;
	WId win_id;

	static std::map<WId, UIWindow *> windows_by_win_id;
	float z_level;
	qreal opacity_level;

	static UIWindow *top_level_window;
	bool isDecorationsNeeded = false;
	bool mIsDecorationUpdateNeeded = true;
};

}

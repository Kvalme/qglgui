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
#include <QRect>
#include <QSvgRenderer>

class QImage;
class QWindow;

namespace QGL
{
	
class GlUIWindowDecoratorAuroraeButton
{
	public:
		virtual bool ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers) = 0;
		QImage* GetImage(QWindow *wnd);
		QRect GetBounds() const { return mBounds;}
		void SetBounds(QRect bounds) { mBounds = bounds;}
		void RenderButton(int width, int height);
		QSvgRenderer mRenderer;
		
		virtual ~GlUIWindowDecoratorAuroraeButton();
	protected:
		struct ButtonCache
		{
			QImage *mActiveCenter = nullptr;
			QImage *mHoverCenter = nullptr;
			QImage *mPressedCenter = nullptr;
			QImage *mDeactivatedCenter = nullptr;
			
			QImage *mInactiveCenter = nullptr;
			QImage *mHoverInactiveCenter = nullptr;
			QImage *mDeactivatedInactiveCenter = nullptr;
		} mCache;

		enum class STATE
		{
			ACTIVE,
			HOVER,
			PRESSED,
			DEACTIVATED,
		} mButtonState = STATE::ACTIVE;
		QRect mBounds;
};

class GlUIWindowDecoratorAuroraeButtonClose : public GlUIWindowDecoratorAuroraeButton
{
public:
		virtual bool ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
};

class GlUIWindowDecoratorAuroraeButtonMinimize : public GlUIWindowDecoratorAuroraeButton
{
public:
		virtual bool ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
};

class GlUIWindowDecoratorAuroraeButtonMaximize : public GlUIWindowDecoratorAuroraeButton
{
public:
		virtual bool ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
};

class GlUIWindowDecoratorAuroraeButtonRestore : public GlUIWindowDecoratorAuroraeButton
{
public:
		virtual bool ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
};

}

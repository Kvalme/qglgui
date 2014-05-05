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

#include "gluiwindowdecoratorauroraebuttons.h"
#include "glguiwindowdecoratoraurorae.h"
#include "libcppprofiler/src/cppprofiler.h"

#include <QWindow>

using namespace QGL;

void GlUIWindowDecoratorAuroraeButton::ResetState()
{
	mButtonState = (mButtonState == STATE::DEACTIVATED ? mButtonState : STATE::ACTIVE);
}

QImage *GlUIWindowDecoratorAuroraeButton::GetImage(QWindow *wnd)
{
	PROFILE_FUNCTION

	bool isActive = wnd->isActive();
	switch(mButtonState)
	{
		case STATE::ACTIVE: return isActive ? mCache.mActiveCenter : mCache.mInactiveCenter;
		case STATE::DEACTIVATED: return isActive ? mCache.mDeactivatedCenter : mCache.mDeactivatedInactiveCenter;
		case STATE::HOVER: return isActive ? mCache.mHoverCenter : mCache.mHoverInactiveCenter;
		case STATE::PRESSED: return isActive ? mCache.mPressedCenter : mCache.mInactiveCenter;
		default: return nullptr;
	}
}

void GlUIWindowDecoratorAuroraeButton::RenderButton(int width, int height)
{
	PROFILE_FUNCTION

	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mActiveCenter), "active-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mHoverCenter), "hover-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mDeactivatedCenter), "deactivated-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mPressedCenter), "pressed-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mDeactivatedInactiveCenter), "deactivated-inactive-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mInactiveCenter), "inactive-center", mRenderer, width, height);
	GlUIWindowDecoratorAurorae::RenderPart(&(mCache.mHoverInactiveCenter), "hover-inactive-center", mRenderer, width, height);
}

GlUIWindowDecoratorAuroraeButton::~GlUIWindowDecoratorAuroraeButton()
{
	PROFILE_FUNCTION
	
	delete mCache.mActiveCenter;
	delete mCache.mHoverCenter;
	delete mCache.mDeactivatedCenter;
	delete mCache.mDeactivatedInactiveCenter;
	delete mCache.mHoverInactiveCenter;
	delete mCache.mInactiveCenter;
	delete mCache.mPressedCenter;
}

bool GlUIWindowDecoratorAuroraeButtonClose::ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	if (mButtonState == STATE::DEACTIVATED) return false;
	
	if (buttons == Qt::NoButton && mButtonState != STATE::PRESSED)
	{
		mButtonState = STATE::HOVER;
		return true;
	}
	else if (buttons == Qt::NoButton && mButtonState == STATE::PRESSED)
	{
		wnd->close();
		return true;
	}
	else if (((buttons & Qt::LeftButton) == Qt::LeftButton))
	{
		mButtonState = STATE::PRESSED;
		return true;
	}
	
	return false;
}

bool GlUIWindowDecoratorAuroraeButtonMaximize::ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	return false;
}

bool GlUIWindowDecoratorAuroraeButtonMinimize::ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	return false;
}

bool GlUIWindowDecoratorAuroraeButtonRestore::ProceedMouse(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	PROFILE_FUNCTION

	return false;
}

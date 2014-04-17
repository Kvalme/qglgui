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
#include <qglgui/glguiwindowdecorator.h>
#include <QColor>
#include <QRect>
#include <QSettings>
#include <QSvgRenderer>

class QImage;

namespace QGL
{

class GlUIWindowDecoratorAurorae : public GlUIWindowDecorator
{
public:
	virtual void SetTheme(const std::string &path, const std::string &name);

private:
	bool LoadGeneral(QSettings &settings);
	bool LoadLayout(QSettings &settings);
	bool LoadResources();
	bool PrecacheResources();
	QColor SplitColor(QStringList color);

	std::string mThemePath;
	std::string mThemeName;
	QColor mActiveTextColor;
	QColor mInactiveTextColor;
	Qt::Alignment mTitleAlignment;
	uint32_t mAnimation;
	uint32_t mBorderLeft;
	uint32_t mBorderRight;
	uint32_t mBorderBottom;
	uint32_t mTitleEdgeTop;
	uint32_t mTitleEdgeBottom;
	uint32_t mTitleEdgeLeft;
	uint32_t mTitleEdgeRight;
	uint32_t mTitleBorderLeft;
	uint32_t mTitleBorderRight;
	uint32_t mTitleHeight;
	uint32_t mButtonWidth;
	uint32_t mButtonHeight;
	uint32_t mButtonSpacing;
	uint32_t mButtonMarginTop;
	uint32_t mExplicitButtonSpacer;
	uint32_t mPaddingTop;
	uint32_t mPaddingBottom;
	uint32_t mPaddingRight;
	uint32_t mPaddingLeft;
	
	QSvgRenderer mCloseButton;
	QSvgRenderer mMaximizeButton;
	QSvgRenderer mMinimizeButton;
	QSvgRenderer mRestoreButton;
	
	QSvgRenderer mDecoration;
	
	struct DecorationCache
	{
		QImage *topLeft = nullptr;
		QImage *topRight = nullptr;
		QImage *bottomLeft = nullptr;
		QImage *bottomRight = nullptr;
		QImage *top = nullptr;
		QImage *right = nullptr;
		QImage *bottom = nullptr;
		QImage *left = nullptr;
	};
	struct ButtonCache
	{
		QImage *activeCenter = nullptr;
		QImage *hoverCenter = nullptr;
		QImage *pressedCenter = nullptr;
		QImage *deactivatedCenter = nullptr;
		
		QImage *inactiveCenter = nullptr;
		QImage *hoverInactiveCenter = nullptr;
		QImage *deactivatedInactiveCenter = nullptr;
	};
	
	DecorationCache mActiveDecoration;
	DecorationCache mInactiveDecoration;
	ButtonCache mCloseButtonCache;
	ButtonCache mMaximizeButtonCache;
	ButtonCache mMinimizeButtonCache;
	ButtonCache mRestoreButtonCache;
	
};

}


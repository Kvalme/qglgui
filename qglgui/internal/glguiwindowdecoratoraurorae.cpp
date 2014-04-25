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

#include "glguiwindowdecoratoraurorae.h"

#include "libcppprofiler/src/cppprofiler.h"
#include <QSettings>
#include <QStringList>
#include <QMargins>
#include <qguiapplication.h>
#include <QImage>
#include <QScreen>
#include <qpaintengine.h>
#include <QWindow>

using namespace QGL;

void GlUIWindowDecoratorAurorae::SetTheme(const std::string &path, const std::string &name)
{
	PROFILE_FUNCTION

	mThemeName = name;
	mThemePath = path + "/" + name + "/";

	QSettings settings((mThemePath + mThemeName + "rc").c_str(), QSettings::IniFormat);
	settings.sync();

	if (settings.status() != QSettings::NoError)
	{
		throw std::runtime_error("Unable to load theme");
	}

	if (!LoadGeneral(settings))throw std::runtime_error("Unable to load theme. Unable to load [General] section");
	if (!LoadLayout(settings))throw std::runtime_error("Unable to load theme. Unable to load [Layout] section");
	if (!LoadResources())throw std::runtime_error("Unable to load theme. Unable to load theme files");
	if (!PrecacheResources())throw std::runtime_error("Unable to load theme. Unable to cache resources");
}

bool GlUIWindowDecoratorAurorae::LoadGeneral(QSettings &settings)
{
	PROFILE_FUNCTION
	
	mActiveTextColor = SplitColor(settings.value("ActiveTextColor").toStringList());
	mInactiveTextColor = SplitColor(settings.value("InactiveTextColor").toStringList());
	QString hAlignment = settings.value("TitleAlignment").toString();
	QString vAlignment = settings.value("TitleVerticalAlignment").toString();
	mTitleAlignment = 0;

	if (hAlignment == "Center") mTitleAlignment = Qt::AlignHCenter;
	else if (hAlignment == "Left") mTitleAlignment = Qt::AlignLeft;
	else if (hAlignment == "Right") mTitleAlignment = Qt::AlignRight;

	if (vAlignment == "Top") mTitleAlignment |= Qt::AlignTop;
	else if (vAlignment == "Bottom") mTitleAlignment |= Qt::AlignBottom;
	else if (vAlignment == "Middle") mTitleAlignment = Qt::AlignVCenter;

	mAnimation = settings.value("Animation").toUInt();
	
	return true;
}

bool GlUIWindowDecoratorAurorae::LoadLayout(QSettings &settings)
{
	PROFILE_FUNCTION
	
	mBorderLeft = settings.value("Layout/BorderLeft").toUInt();
	mBorderRight = settings.value("Layout/BorderRight").toUInt();
	mBorderBottom = settings.value("Layout/BorderBottom").toUInt();
	mTitleEdgeTop = settings.value("Layout/TitleEdgeTop").toUInt();
	mTitleEdgeBottom = settings.value("Layout/TitleEdgeBottom").toUInt();
	mTitleEdgeLeft = settings.value("Layout/TitleEdgeLeft").toUInt();
	mTitleEdgeRight = settings.value("Layout/TitleEdgeRight").toUInt();
	mTitleBorderLeft = settings.value("Layout/TitleBorderLeft").toUInt();
	mTitleBorderRight = settings.value("Layout/TitleBorderRight").toUInt();
	mTitleHeight = settings.value("Layout/TitleHeight").toUInt();
	mButtonWidth = settings.value("Layout/ButtonWidth").toUInt();
	mButtonHeight = settings.value("Layout/ButtonHeight").toUInt();
	mButtonSpacing = settings.value("Layout/ButtonSpacing").toUInt();
	mButtonMarginTop = settings.value("Layout/ButtonMarginTop").toUInt();
	mExplicitButtonSpacer = settings.value("Layout/ExplicitButtonSpacer").toUInt();
	mPaddingTop = settings.value("Layout/PaddingTop").toUInt();
	mPaddingBottom = settings.value("Layout/PaddingBottom").toUInt();
	mPaddingRight = settings.value("Layout/PaddingRight").toUInt();
	mPaddingLeft = settings.value("Layout/PaddingLeft").toUInt();
	
	return true;
}

QColor GlUIWindowDecoratorAurorae::SplitColor(QStringList color)
{
	PROFILE_FUNCTION
	
	QColor c;
	c.setRed(color[0].toUInt());
	c.setGreen(color[1].toUInt());
	c.setBlue(color[2].toUInt());
	c.setAlpha(color[3].toUInt());
	
	return c;
}

bool GlUIWindowDecoratorAurorae::LoadResources()
{
	PROFILE_FUNCTION
	
	if (!mCloseButton.load(QString((mThemePath + "close.svgz").c_str())) && !mCloseButton.load(QString((mThemePath + "close.svg").c_str()))) return false;
	if (!mMaximizeButton.load(QString((mThemePath + "maximize.svgz").c_str())) && !mMaximizeButton.load(QString((mThemePath + "maximize.svg").c_str()))) return false;
	if (!mMinimizeButton.load(QString((mThemePath + "minimize.svgz").c_str())) && !mMinimizeButton.load(QString((mThemePath + "minimize.svg").c_str()))) return false;
	if (!mRestoreButton.load(QString((mThemePath + "restore.svgz").c_str())) && !mRestoreButton.load(QString((mThemePath + "restore.svg").c_str()))) return false;
	if (!mDecoration.load(QString((mThemePath + "decoration.svgz").c_str())) && !mDecoration.load(QString((mThemePath + "decoration.svg").c_str()))) return false;
	return true;
}

void GlUIWindowDecoratorAurorae::renderPart(QImage **image, const QString &elementId, QSvgRenderer &source, int width, int height)
{
		*image = new QImage(width, height, QImage::Format_ARGB32);
		(*image)->fill(QColor(0, 0, 0, 0));
		QPainter imagePainter(*image);
		source.render(&imagePainter, elementId);
}

void GlUIWindowDecoratorAurorae::renderButton(GlUIWindowDecoratorAurorae::ButtonCache &cache, QSvgRenderer &source)
{
		renderPart(&cache.activeCenter, "active-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.hoverCenter, "hover-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.deactivatedCenter, "deactivated-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.pressedCenter, "pressed-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.deactivatedInactiveCenter, "deactivated-inactive-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.inactiveCenter, "inactive-center", source, mButtonWidth, mButtonHeight);
		renderPart(&cache.hoverInactiveCenter, "hover-inactive-center", source, mButtonWidth, mButtonHeight);
}


bool GlUIWindowDecoratorAurorae::PrecacheResources()
{
	PROFILE_FUNCTION
	
	renderButton(mCloseButtonCache, mCloseButton);
	renderButton(mMaximizeButtonCache, mMaximizeButton);
	renderButton(mMinimizeButtonCache, mMinimizeButton);
	renderButton(mRestoreButtonCache, mRestoreButton);
	
	//Prepare decoration fixed-sized blocks
	renderPart(&mActiveDecoration.topLeft, "decoration-topleft", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mActiveDecoration.topRight, "decoration-topright", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mActiveDecoration.bottomLeft, "decoration-bottomleft", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mActiveDecoration.bottomRight, "decoration-bottomright", mDecoration, mTitleHeight, mTitleHeight);
	
	renderPart(&mInactiveDecoration.topLeft, "decoration-inactive-topleft", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mInactiveDecoration.topRight, "decoration-inactive-topright", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mInactiveDecoration.bottomLeft, "decoration-inactive-bottomleft", mDecoration, mTitleHeight, mTitleHeight);
	renderPart(&mInactiveDecoration.bottomRight, "decoration-inactive-bottomright", mDecoration, mTitleHeight, mTitleHeight);
	
	return true;
}

QMargins GlUIWindowDecoratorAurorae::GetFrameMargins()
{
	PROFILE_FUNCTION
	
	return QMargins(mBorderLeft, mTitleHeight, mBorderRight, mBorderBottom);
}

void GlUIWindowDecoratorAurorae::Render(QWindow *window, QPaintDevice *image)
{
	PROFILE_FUNCTION
	
	//Render fixed parts
	QPainter painter(image);
	QRect geom = window->geometry();
//	QMargins m = window->frameMargins();
	DecorationCache *decoration = window->isActive() ? &mActiveDecoration : &mInactiveDecoration;
	
	CacheBorders(window->isActive(), geom.width(), geom.height());
	
	//corners
	QPoint target(0, 0);
	painter.drawImage(target, *(decoration->topLeft));
	
 	target.setX(mBorderLeft + geom.width());
	painter.drawImage(target, *(decoration->topRight));
	
	target.setX(0);
	target.setY(mTitleHeight + geom.height());
	painter.drawImage(target, *(decoration->bottomLeft));
	
	target.setX(mBorderLeft + geom.width());
	painter.drawImage(target, *(decoration->bottomRight));
	
	//draw borders
	target.setX(mBorderLeft);
	target.setY(0);
	painter.drawImage(target, *(decoration->top));

	target.setX(mBorderLeft);
	target.setY(mTitleHeight + geom.height());
	painter.drawImage(target, *(decoration->bottom));

	target.setX(0);
	target.setY(mTitleHeight);
	painter.drawImage(target, *(decoration->left));

	target.setX(mBorderLeft + geom.width());
	target.setY(mTitleHeight);
	painter.drawImage(target, *(decoration->right));

}

void GlUIWindowDecoratorAurorae::CacheBorders(bool isActive, int width, int height)
{
	PROFILE_FUNCTION
	
	if (isActive)
	{
		renderPart(&mActiveDecoration.top, "decoration-top", mDecoration, width, mTitleHeight);
		renderPart(&mActiveDecoration.bottom, "decoration-bottom", mDecoration, width, mBorderBottom);
		renderPart(&mActiveDecoration.left, "decoration-left", mDecoration, mBorderLeft, height);
		renderPart(&mActiveDecoration.right, "decoration-right", mDecoration, mBorderRight, height);
	}
	else
	{
		renderPart(&mInactiveDecoration.top, "decoration-inactive-top", mDecoration, width, mTitleHeight);
		renderPart(&mInactiveDecoration.bottom, "decoration-inactive-bottom", mDecoration, width, mBorderBottom);
		renderPart(&mInactiveDecoration.left, "decoration-inactive-left", mDecoration, mBorderLeft, height);
		renderPart(&mInactiveDecoration.right, "decoration-inactive-right", mDecoration, mBorderRight, height);
	}
}


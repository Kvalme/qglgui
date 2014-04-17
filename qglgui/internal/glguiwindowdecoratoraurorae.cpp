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
#include <qguiapplication.h>
#include <QImage>
#include <QScreen>
#include <qpaintengine.h>

#include <iostream>

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
	QColor c;
	c.setRed(color[0].toUInt());
	c.setGreen(color[1].toUInt());
	c.setBlue(color[2].toUInt());
	c.setAlpha(color[3].toUInt());
	
	return c;
}

bool GlUIWindowDecoratorAurorae::LoadResources()
{
	if (!mCloseButton.load(QString((mThemePath + "close.svgz").c_str())) && !mCloseButton.load(QString((mThemePath + "close.svg").c_str()))) return false;
	if (!mMaximizeButton.load(QString((mThemePath + "maximize.svgz").c_str())) && !mMaximizeButton.load(QString((mThemePath + "maximize.svg").c_str()))) return false;
	if (!mMinimizeButton.load(QString((mThemePath + "minimize.svgz").c_str())) && !mMinimizeButton.load(QString((mThemePath + "minimize.svg").c_str()))) return false;
	if (!mRestoreButton.load(QString((mThemePath + "restore.svgz").c_str())) && !mRestoreButton.load(QString((mThemePath + "restore.svg").c_str()))) return false;
	if (!mDecoration.load(QString((mThemePath + "decoration.svgz").c_str())) && !mDecoration.load(QString((mThemePath + "decoration.svg").c_str()))) return false;
	return true;
}

bool GlUIWindowDecoratorAurorae::PrecacheResources()
{
	auto renderButtonPart = [&](QImage **image, const QString &elementId, QSvgRenderer &source)
	{
		*image = new QImage(mButtonWidth, mButtonHeight, QImage::Format_ARGB32);
		(*image)->fill(QColor(0, 0, 0, 0));
		QPainter imagePainter(*image);
		source.render(&imagePainter, elementId);	
	};
	auto renderButton = [&](ButtonCache &cache, QSvgRenderer &source)
	{
		renderButtonPart(&cache.activeCenter, "active-center", source);
		renderButtonPart(&cache.hoverCenter, "hover-center", source);
		renderButtonPart(&cache.deactivatedCenter, "deactivated-center", source);
		renderButtonPart(&cache.pressedCenter, "pressed-center", source);
		renderButtonPart(&cache.deactivatedInactiveCenter, "deactivated-inactive-center", source);
		renderButtonPart(&cache.inactiveCenter, "inactive-center", source);
		renderButtonPart(&cache.hoverInactiveCenter, "hover-inactive-center", source);
	};
	
	renderButton(mCloseButtonCache, mCloseButton);
	renderButton(mMaximizeButtonCache, mMaximizeButton);
	renderButton(mMinimizeButtonCache, mMinimizeButton);
	renderButton(mRestoreButtonCache, mRestoreButton);
	
	
	return true;
}

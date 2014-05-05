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
#include "qtplugin/uiwindow.h"
#include "gluiwindowdecoratorauroraebuttons.h"

#include "libcppprofiler/src/cppprofiler.h"
#include <QSettings>
#include <QStringList>
#include <QMargins>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QScreen>
#include <QPaintEngine>
#include <QPainter>
#include <QWindow>
#include <qevent.h>
#include <qpa/qwindowsysteminterface.h>

using namespace QGL;

void GlUIWindowDecoratorAurorae::SetTheme(const std::string &path, const std::string &name)
{
	PROFILE_FUNCTION

	mIsUpdateNeeded = true;
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

GlUIWindowDecoratorAurorae::~GlUIWindowDecoratorAurorae()
{
	PROFILE_FUNCTION

	delete mTitleFontMetrics;
	for (unsigned int a = 0; a < mButtons.size(); ++a)
	{
		delete mButtons[a];
	}
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

	auto loadFile = [&](std::string name, QSvgRenderer & renderer)
	{
		if (QFile::exists(QString((mThemePath + name + ".svgz").c_str())))
			if (!renderer.load(QString((mThemePath + name + ".svgz").c_str()))) return false;

		if (QFile::exists(QString((mThemePath + name + ".svg").c_str())))
			if (!renderer.load(QString((mThemePath + name + ".svg").c_str()))) return false;

		return true;
	};

	mButtons.clear();
	GlUIWindowDecoratorAuroraeButton *button;
	button = new GlUIWindowDecoratorAuroraeButtonClose;
	if (!loadFile("close", button->mRenderer))return false;
	mButtons.push_back(button);
/*	if (!loadFile("maximize", mMaximizeButton))return false;
	if (!loadFile("minimize", mMinimizeButton))return false;
	if (!loadFile("restore", mRestoreButton))return false;*/
	if (!loadFile("decoration", mDecoration))return false;

	return true;
}

void GlUIWindowDecoratorAurorae::RenderPart(QImage **image, const QString &elementId, QSvgRenderer &source, int width, int height)
{
	PROFILE_FUNCTION

	delete *image;
	*image = new QImage(width, height, QImage::Format_ARGB32);
	(*image)->fill(QColor(0, 0, 0, 0));
	QPainter imagePainter(*image);
	source.render(&imagePainter, elementId);
}

bool GlUIWindowDecoratorAurorae::PrecacheResources()
{
	PROFILE_FUNCTION

	mTitleFont = QFont("Arial", mTitleHeight);
	mTitleFontMetrics = new QFontMetrics(mTitleFont);

	for(GlUIWindowDecoratorAuroraeButton *button : mButtons)
	{
		button->RenderButton(mButtonWidth, mButtonHeight);
	}

	return true;
}

QMargins GlUIWindowDecoratorAurorae::GetFrameMargins()
{
	PROFILE_FUNCTION

	return QMargins(mBorderLeft, mTitleFontMetrics->height() + mTitleEdgeBottom + mTitleEdgeTop, mBorderRight, mBorderBottom);
}

void GlUIWindowDecoratorAurorae::Render(QWindow *window, QPaintDevice *image)
{
	PROFILE_FUNCTION

	mIsUpdateNeeded = false;

	RenderFrame(window, image);

	QPainter painter(image);

	//draw buttons
//	int bid = 0;
	for(GlUIWindowDecoratorAuroraeButton *button : mButtons)
	{
		QRect bounds = QRect(mBorderLeft + window->width() - mButtonSpacing - mButtonWidth, mPaddingTop, mButtonWidth, mButtonHeight);
		button->SetBounds(bounds);
		QPoint target(bounds.x(), bounds.y());
		painter.drawImage(target, *(button->GetImage(window)));
	}

	//draw title
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
	QColor textColor = window->isActive() ? mActiveTextColor : mInactiveTextColor;
	QPoint target(mBorderLeft + mPaddingLeft, mPaddingTop);
	QRect rect(target, QSize(window->width(), mTitleFontMetrics->height()));
	painter.setPen(textColor);
	painter.setBrush(QBrush(textColor));
	painter.setFont(mTitleFont);
	painter.drawText(rect, mTitleAlignment, window->title());
}

void GlUIWindowDecoratorAurorae::RenderFrame(QWindow *window, QPaintDevice *image)
{
	PROFILE_FUNCTION

	QString prefix("decoration-");
	if (!window->isActive()) prefix += "inactive-";

	const int right = window->width() + mBorderLeft;
	const int titleHeight = mTitleFontMetrics->height() + mTitleEdgeBottom + mTitleEdgeTop;
	const int bottom = window->height() + titleHeight;

	QPoint target(0, 0);
	QPainter imagePainter(image);
	imagePainter.setCompositionMode(QPainter::CompositionMode_Source);

	//Clean up image first
	imagePainter.fillRect(QRect(0, 0, window->frameGeometry().width(), titleHeight), QColor(0, 0, 0, 0));
	imagePainter.fillRect(QRect(0, 0, mBorderLeft, window->frameGeometry().height()), QColor(0, 0, 0, 0));
	imagePainter.fillRect(QRect(right, 0, mBorderRight, window->frameGeometry().height()), QColor(0, 0, 0, 0));
	imagePainter.fillRect(QRect(0, bottom, window->frameGeometry().width(), mBorderBottom), QColor(0, 0, 0, 0));

	imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	mBounds.mTopLeft = QRect(QPoint(0, 0), QSize(mBorderLeft, titleHeight));
	mBounds.mTitleBar = QRect(QPoint(mBorderLeft, 0), QSize(window->width(), titleHeight));
	mBounds.mTopRight = QRect(QPoint(right, 0), QSize(mBorderRight, titleHeight));

	mBounds.mLeft = QRect(QPoint(0, titleHeight), QSize(mBorderLeft, window->height()));
	mBounds.mRight = QRect(QPoint(right, titleHeight), QSize(mBorderRight, window->height()));

	mBounds.mBottomLeft = QRect(QPoint(0, bottom), QSize(mBorderLeft, mBorderBottom));
	mBounds.mBottom = QRect(QPoint(mBorderLeft, bottom), QSize(window->width(), mBorderBottom));
	mBounds.mBottomRight = QRect(QPoint(right, bottom), QSize(mBorderRight, mBorderBottom));

	mDecoration.render(&imagePainter, prefix + "topleft", mBounds.mTopLeft);
	mDecoration.render(&imagePainter, prefix + "top", mBounds.mTitleBar);
	mDecoration.render(&imagePainter, prefix + "topright", mBounds.mTopRight);

	mDecoration.render(&imagePainter, prefix + "left", mBounds.mLeft);
	mDecoration.render(&imagePainter, prefix + "right", mBounds.mRight);

	mDecoration.render(&imagePainter, prefix + "bottomleft", mBounds.mBottomLeft);
	mDecoration.render(&imagePainter, prefix + "bottom", mBounds.mBottom);
	mDecoration.render(&imagePainter, prefix + "bottomright", mBounds.mBottomRight);
}

bool GlUIWindowDecoratorAurorae::IsDecorationChanged() const
{
	return mIsUpdateNeeded;
}

bool GlUIWindowDecoratorAurorae::Bounds::IsInsideBounds(QPoint local)
{
	return mTopLeft.contains(local) || mTitleBar.contains(local) || mTopRight.contains(local) ||
		   mRight.contains(local) || mLeft.contains(local) ||
		   mBottomLeft.contains(local) || mBottom.contains(local) || mBottomRight.contains(local);
}


bool GlUIWindowDecoratorAurorae::handleMouseEvent(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	//Correct local position accroding to decoration size
	local.setX(local.x() + mBorderLeft);
	local.setY(local.y() + mBounds.mTitleBar.height());

	//If we are not inside decoration - ignore event
	if (!mBounds.IsInsideBounds(local))return false;

	//Check for buttons first
	for (GlUIWindowDecoratorAuroraeButton *button : mButtons)
	{
		if (button->GetBounds().contains(local))
		{
			if (button->ProceedMouse(wnd, local, position, buttons, modifiers))
			{
				QWindowSystemInterface::handleExposeEvent(wnd, QRegion(wnd->geometry()));
				mIsUpdateNeeded = true;
				return true;
			}
		}
		else
			button->ResetState();
	}
	
	if (mBounds.mTitleBar.contains(local))
	{
		ProceedTitlebarActions(wnd, local, position, buttons, modifiers);
		return true;
	}
	else if(mBounds.mBottom.contains(local))
	{
		ProceedResizeActions(wnd, local, position, buttons, modifiers);
		return true;
	}
	
	return false;
}

void GlUIWindowDecoratorAurorae::ProceedTitlebarActions(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	UIWindow *window = static_cast<UIWindow*>(wnd->handle());
	
	if (buttons == Qt::NoButton && window->IsMoving()) window->MarkAsMoving(false);
	if (((buttons & Qt::LeftButton) == Qt::LeftButton))
	{
		if (!window->IsMoving())
		{
			window->MarkAsMoving(true, position);
		}
		else
		{
			QPoint oldPos = window->GetMoveStartPosition();
			QPoint delta = position - oldPos;
			QRect geom = wnd->geometry();
			geom.translate(delta.x(), delta.y());
			wnd->setGeometry(geom);
			window->MarkAsMoving(true, position);
		}
	}
}

void GlUIWindowDecoratorAurorae::ProceedResizeActions(QWindow *wnd, QPoint local, QPoint position, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
//	UIWindow *window = static_cast<UIWindow*>(wnd->handle());

//	if (buttons == Qt::NoButton && window->IsMoving()) window->MarkAsResizing(false);
	
	
}

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

#include "gl1guirenderer.h"

#include "libcppprofiler/src/cppprofiler.h"

#include <GL/gl.h>
#include <assert.h>
#include <QWindow>
#include <QScreen>

#include "qglgui/internal/qtplugin/uiwindow.h"

using namespace QGL;

Gl1GuiRenderer::Gl1GuiRenderer()
{
	PROFILE_FUNCTION

}

Gl1GuiRenderer::~Gl1GuiRenderer()
{
	PROFILE_FUNCTION

	for (auto entry : mWindows)
	{
		delete[] entry.second.mTextureBuffer;
	}
}

void Gl1GuiRenderer::Render(std::mutex *mutex)
{
	PROFILE_FUNCTION

	if (mutex) mutex->lock();
	RemoveUneededWindows();
	auto windowsCopy = mWindows;
	if (mutex) mutex->unlock();

	GLboolean blendStatus = glIsEnabled(GL_BLEND);
	GLboolean tex2DStatus = glIsEnabled(GL_TEXTURE_2D);

	if (tex2DStatus != GL_TRUE) glEnable(GL_TEXTURE_2D);
	if (blendStatus != GL_TRUE) glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); //Save matrix

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //Save current matrix

	//Render all windows
	for (auto & entry : windowsCopy)
	{
		if (!entry.second.mWindow->isVisible())continue;

		glLoadIdentity();
		UpdateTexture(&entry.second);
		RenderWindow(entry.second);
	}

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	if (tex2DStatus == GL_FALSE) glDisable(GL_TEXTURE_2D);
	if (blendStatus == GL_FALSE) glDisable(GL_BLEND);
}

void Gl1GuiRenderer::RemoveUneededWindows()
{
	PROFILE_FUNCTION

	for (auto it = mWindows.begin(); it != mWindows.end();)
	{
		if (it->second.mIsRemoveOnRender)
		{
			glDeleteTextures(1, &it->second.mTexId);
			delete[] it->second.mTextureBuffer;
			mWindows.erase(it++);
		}
		else ++it;
	}
}

void Gl1GuiRenderer::RenderWindow(const Gl1GuiRenderer::WindowRenderInformation &window)
{
	PROFILE_FUNCTION

	assert(window.mWindow != nullptr);

	QRect viewport = window.mWindow->window()->screen()->availableGeometry();

	glViewport(viewport.x(), viewport.y(), viewport.width(), viewport.height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(viewport.x(), viewport.width(), viewport.height(), viewport.y(), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, window.mTexId);

	QRect geometry = window.mWindow->geometry();

	float xs = geometry.x();
	float ys = geometry.y();
	float xe = xs + geometry.width();
	float ye = ys + geometry.height();
	float z_pos = window.mWindow->getZLevel();
	float alpha = window.mWindow->getOpacity();

	glBegin(GL_QUADS);

	glColor4f(1.0, 1.0, 1.0, alpha);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(xs, ys, z_pos);

	glColor4f(1.0, 1.0, 1.0, alpha);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(xs, ye, z_pos);

	glColor4f(1.0, 1.0, 1.0, alpha);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(xe, ye, z_pos);

	glColor4f(1.0, 1.0, 1.0, alpha);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(xe, ys, z_pos);

	glEnd();
}

void Gl1GuiRenderer::UpdateTexture(Gl1GuiRenderer::WindowRenderInformation *window)
{
	PROFILE_FUNCTION

	if (!window->mIsTextureChanged)return;

	if (glIsTexture(window->mTexId) == GL_FALSE)
	{
		glGenTextures(1, &window->mTexId);
	}

	glBindTexture(GL_TEXTURE_2D, window->mTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->mWindow->window()->width(),
				 window->mWindow->window()->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, window->mTextureBuffer);

	window->mIsTextureChanged = false;
}

void Gl1GuiRenderer::GuiCreateWindow(unsigned int winId, UIWindow *wnd)
{
	PROFILE_FUNCTION

	assert(wnd);
	assert(mWindows.find(winId) == mWindows.end());

	WindowRenderInformation wri(wnd, winId);

	mWindows.insert(std::make_pair(winId, wri));
}

void Gl1GuiRenderer::GuiRemoveWindow(unsigned int winId)
{
	PROFILE_FUNCTION

	auto it = mWindows.find(winId);
	assert(it != mWindows.end());

	it->second.mIsRemoveOnRender = true;
}

void Gl1GuiRenderer::GuiSetTexture(unsigned int winId, QPixmap pixmap)
{
	PROFILE_FUNCTION

	auto it = mWindows.find(winId);
	assert(it != mWindows.end());

	QImage img = pixmap.toImage();

	it->second.mIsTextureChanged = true;
	delete[] it->second.mTextureBuffer;
	it->second.mTextureBuffer = new unsigned char[pixmap.width() * pixmap.height() * 4];

	memcpy(
		static_cast<void *>(it->second.mTextureBuffer),
		static_cast<void *>(img.bits()),
		pixmap.width() * pixmap.height() * 4);
}

Gl1GuiRenderer::WindowRenderInformation::WindowRenderInformation() :
	mWindow(nullptr),
	mWinId(0),
	mTexId(0),
	mIsRemoveOnRender(false),
	mIsTextureChanged(true),
	mTextureBuffer(nullptr)
{
	PROFILE_FUNCTION
}

Gl1GuiRenderer::WindowRenderInformation::WindowRenderInformation(UIWindow *window, unsigned int winId) :
	mWindow(window),
	mWinId(winId),
	mTexId(0),
	mIsRemoveOnRender(false),
	mIsTextureChanged(true),
	mTextureBuffer(nullptr)
{
	PROFILE_FUNCTION
}

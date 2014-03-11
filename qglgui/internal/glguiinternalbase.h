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

#include "qglgui/glgui.h"

#include <QPixmap>
#include <map>

namespace QGL
{
class UIWindow;
class GlGuiInternalBase : public GlGui
{
public:
	GlGuiInternalBase(const std::string &fontDir);
	virtual ~GlGuiInternalBase();
	
	virtual int CreateScreen(QRect viewport) = 0;
	virtual void CreateWindow(const std::string &name) = 0;
	virtual void RegisterWindowFactory(std::function<QWidget*(const std::string &name)> factory);
	virtual void RegisterRenderer(std::shared_ptr<GlGuiRenderer> renderer);
	virtual void Render() = 0;
	virtual void Update() = 0;

	//Internal interface
	virtual void iAddWindow(UIWindow *wnd);
	virtual void iSetTexture(unsigned int winId, const QPixmap &pixmap);
	virtual void iRemoveWindow(unsigned int winId);

protected:
	struct Window
	{
		UIWindow *wnd;
		
		explicit Window(UIWindow *w = nullptr);
	};

	std::function<QWidget*(const std::string &name)> windowFactory;
	std::shared_ptr<GlGuiRenderer> guiRenderer;
	
	std::map<unsigned int, Window> windows;
};

}

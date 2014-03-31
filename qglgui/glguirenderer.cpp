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

#include "qglgui/glguirenderer.h"
#include "internal/gl1guirenderer.h"

#include "libcppprofiler/src/cppprofiler.h"

using namespace QGL;

GlGuiRenderer * QGL::CreateRenderer(RENDERER_TYPE type)
{
	PROFILE_FUNCTION

	switch (type)
	{
		case RENDERER_TYPE::GL1:
			return new Gl1GuiRenderer;
		case RENDERER_TYPE::GL2:
			throw std::runtime_error("GL2 renderer is no implemented");
			break;
		case RENDERER_TYPE::GL3:
			throw std::runtime_error("GL3 renderer is no implemented");
			break;
	}
	throw std::runtime_error("Invalid renderer");
	return nullptr;
}


GlGuiRenderer::GlGuiRenderer()
{
	PROFILE_FUNCTION
}

GlGuiRenderer::~GlGuiRenderer()
{
	PROFILE_FUNCTION
}

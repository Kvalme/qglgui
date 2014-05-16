/*
 *  Copyright (c) 2014, Denis Biryukov <denis.birukov@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *      * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY Denis Biryukov <denis.birukov@gmail.com> ''AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL Denis Biryukov <denis.birukov@gmail.com> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include "uiopenglcontext.h"
#include "libcppprofiler/src/cppprofiler.h"
#include "uiintegration.h"

#include <iostream>

using namespace QGL;

GLAPI void GLAPIENTRY glEnable( GLenum cap )
{
	std::cerr<<"glEnable:"<<UIIntegration::guiThreadId<<" "<<std::this_thread::get_id()<<std::endl;
	if (UIIntegration::guiThreadId != std::this_thread::get_id()) ::glEnable(cap);
}
GLAPI void GLAPIENTRY glDisable( GLenum cap )
{
	std::cerr<<"glDisable:"<<UIIntegration::guiThreadId<<" "<<std::this_thread::get_id()<<std::endl;
}

QFunctionPointer UIOpenGLContext::getProcAddress(const QByteArray &procName)
{
	PROFILE_FUNCTION

	std::cerr<<"Requested function:"<<procName.constData()<<std::endl;
	
	return NULL;
}

void UIOpenGLContext::doneCurrent()
{
	PROFILE_FUNCTION
}

bool UIOpenGLContext::makeCurrent(QPlatformSurface *surface)
{
	PROFILE_FUNCTION
	
	return true;
}

void UIOpenGLContext::swapBuffers(QPlatformSurface *surface)
{
	PROFILE_FUNCTION

}

QSurfaceFormat UIOpenGLContext::format() const
{
	PROFILE_FUNCTION
	
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(16);
	fmt.setRedBufferSize(8);
	fmt.setGreenBufferSize(8);
	fmt.setBlueBufferSize(8);
	fmt.setAlphaBufferSize(8);
	fmt.setStencilBufferSize(8);
	
	return fmt;
}

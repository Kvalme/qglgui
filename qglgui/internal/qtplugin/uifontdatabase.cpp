/**********************************************************************
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
 **********************************************************************/

#include "uifontdatabase.h"

#if defined(Q_OS_UNIX)
#if defined(Q_OS_MAC)
#include <qpa/qplatformfontdatabase.h>
#else
#include <QtFontDatabaseSupport/private/qgenericunixfontdatabase_p.h>
#endif
#elif defined(Q_OS_WIN)
#include <QtPlatformSupport/private/qbasicfontdatabase_p.h>
#endif

#include "libcppprofiler/src/cppprofiler.h"

using namespace QGL;

UIFontDatabase::UIFontDatabase()
{
#if defined(Q_OS_UNIX)
#if defined(Q_OS_MAC)
	mFontDatabase = new QPlatformFontDatabase();
#else
	mFontDatabase = new QGenericUnixFontDatabase();
#endif
#elif defined(Q_OS_WIN)
	mFontDatabase = new QBasicFontDatabase();
#endif

}

UIFontDatabase::~UIFontDatabase()
{
	PROFILE_FUNCTION

	delete mFontDatabase;
}

QStringList UIFontDatabase::addApplicationFont(const QByteArray &fontData, const QString &fileName)
{
	PROFILE_FUNCTION

	return mFontDatabase->addApplicationFont(fontData, fileName);
}

QFont UIFontDatabase::defaultFont() const
{
	PROFILE_FUNCTION

	QFont fnt = mFontDatabase->defaultFont();
	fnt.setPointSize(11);
	return fnt;
}

QStringList UIFontDatabase::fallbacksForFamily(const QString &family, QFont::Style style, QFont::StyleHint styleHint, QChar::Script script) const
{
	PROFILE_FUNCTION

	return mFontDatabase->fallbacksForFamily(family, style, styleHint, script);
}

QString UIFontDatabase::fontDir() const
{
	PROFILE_FUNCTION

	return mFontDatabase->fontDir();
}

QFontEngine *UIFontDatabase::fontEngine(const QFontDef &fontDef, void *handle)
{
	PROFILE_FUNCTION

	return mFontDatabase->fontEngine(fontDef, handle);
}

QFontEngine *UIFontDatabase::fontEngine(const QByteArray &fontData, qreal pixelSize, QFont::HintingPreference hintingPreference)
{
	PROFILE_FUNCTION

	return mFontDatabase->fontEngine(fontData, pixelSize, hintingPreference);
}

QFontEngineMulti *UIFontDatabase::fontEngineMulti(QFontEngine *fontEngine, QChar::Script script)
{
	PROFILE_FUNCTION

	return mFontDatabase->fontEngineMulti(fontEngine, script);
}

bool UIFontDatabase::fontsAlwaysScalable() const
{
	PROFILE_FUNCTION

	return mFontDatabase->fontsAlwaysScalable();
}

void UIFontDatabase::populateFontDatabase()
{
	PROFILE_FUNCTION

	mFontDatabase->populateFontDatabase();
}

void UIFontDatabase::releaseHandle(void *handle)
{
	PROFILE_FUNCTION

	mFontDatabase->releaseHandle(handle);
}

QString UIFontDatabase::resolveFontFamilyAlias(const QString &family) const
{
	PROFILE_FUNCTION

	return mFontDatabase->resolveFontFamilyAlias(family);
}

QList< int > UIFontDatabase::standardSizes() const
{
	PROFILE_FUNCTION

	return mFontDatabase->standardSizes();
}

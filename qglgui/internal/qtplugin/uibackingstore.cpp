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

#include "uibackingstore.h"
#include "libcppprofiler/src/cppprofiler.h"
#include "uiintegration.h"
#include <qglgui/internal/glguiinternalbase.h>
#include <qguiapplication.h>
#include <QScreen>
#include <private/qguiapplication_p.h>

using namespace QGL;

std::map<WId, UIBackingStore *> UIBackingStore::win_id_to_backing_store_map;

extern void qt_scrollRectInImage(QImage &img, const QRect &rect, const QPoint &offset);

UIBackingStore::UIBackingStore(QWindow *window) : QPlatformBackingStore(window)
{
	PROFILE_FUNCTION

}

UIBackingStore::~UIBackingStore()
{
	PROFILE_FUNCTION

	clearMap();
}

UIBackingStore *UIBackingStore::backingStoreByWinId(WId id)
{
	PROFILE_FUNCTION

	auto fnd = win_id_to_backing_store_map.find(id);
	return (fnd == win_id_to_backing_store_map.end() ? nullptr : fnd->second);
}

void UIBackingStore::clearMap()
{
	PROFILE_FUNCTION

	for (auto winareas : window_area_map)
	{
		auto backing_store = win_id_to_backing_store_map.find(winareas.first);
		if (backing_store != win_id_to_backing_store_map.end() && backing_store->second == this)
		{
			win_id_to_backing_store_map.erase(backing_store);
		}
	}
	window_area_map.clear();
}

void UIBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
	PROFILE_FUNCTION

	Q_UNUSED(region);

	QSize imageSize = image.size();
	if (imageSize.isEmpty()) return;

	QRegion clipped = QRect(0, 0, window->width(), window->height());
	clipped &= QRect(0, 0, imageSize.width(), imageSize.height()).translated(-offset);

	QRect bounds = clipped.boundingRect().translated(offset);
	if (bounds.isNull()) return;

	WId id = window->winId();

	window_area_map[id] = bounds;
	win_id_to_backing_store_map[id] = this;

	QPixmap pix = grabWindow(id, bounds);

	UIIntegration *i = static_cast<UIIntegration *> (QGuiApplicationPrivate::platform_integration);
	i->getUi()->iSetTexture(id, pix);
}

QPaintDevice *UIBackingStore::paintDevice()
{
	PROFILE_FUNCTION

	return &image;
}

void UIBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
	PROFILE_FUNCTION

	QImage::Format format = QGuiApplication::primaryScreen()->handle()->format();
	if (image.size() != size)
	{
		image = QImage(size, format);
	}
	clearMap();
}

bool UIBackingStore::scroll(const QRegion &area, int dx, int dy)
{
	PROFILE_FUNCTION

	if (image.isNull()) return false;

	const QVector<QRect> rects = area.rects();
	for (const QRect & rect : rects)
	{
		qt_scrollRectInImage(image, rect, QPoint(dx, dy));
	}

	return true;
}

QPixmap UIBackingStore::grabWindow(WId window, const QRect &rect) const
{
	PROFILE_FUNCTION

	auto area_it = window_area_map.find(window);
	if (area_it == window_area_map.end()) return QPixmap();

	QRect area = area_it->second;

	if (area.isNull()) return QPixmap();

	QRect adjusted = rect;
	if (adjusted.width() <= 0)
		adjusted.setWidth(area.width());

	if (adjusted.height() <= 0)
		adjusted.setHeight(area.height());

	adjusted = adjusted.translated(area.topLeft()) & area;

	if (adjusted.isEmpty()) return QPixmap();

	return QPixmap::fromImage(image.copy(adjusted));
}

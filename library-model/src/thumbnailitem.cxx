/* Copyright © 2010 Євгеній Мещеряков <eugen@debian.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "thumbnailitem.h"
#include "thumbnailerservice.h"
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>

ThumbnailerService *ThumbnailItem::m_thumbnailer;

ThumbnailItem::ThumbnailItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_loaded(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    if (!m_thumbnailer)
        m_thumbnailer = new ThumbnailerService(QCoreApplication::instance());

    connect(m_thumbnailer, SIGNAL(thumbnailReady(QString,QPixmap)), SLOT(thumbnailReady(QString,QPixmap)));
}

bool ThumbnailItem::loaded() const
{
    return m_loaded;
}

QString ThumbnailItem::file() const
{
    return m_fileName;
}

void ThumbnailItem::setFile(const QString &fileName)
{
    if (fileName != m_fileName) {
        m_fileName = fileName;
        Q_EMIT fileChanged();

        if (m_loaded) {
            m_loaded = false;
            Q_EMIT loadStatusChanged();
        }

        setImage(QPixmap());
        m_thumbnailer->getThumbnail(fileName);
    }
}

void ThumbnailItem::thumbnailReady(const QString &fileName, const QPixmap &img)
{
    if (fileName == m_fileName)
        setImage(img);
}

void ThumbnailItem::setImage(const QPixmap &img)
{
    m_img = img;
    if (m_img.isNull()) {
        setImplicitWidth(0);
        setImplicitHeight(0);
    } else {
        setImplicitWidth(img.width());
        setImplicitHeight(img.height());
        m_loaded = true;
        Q_EMIT loadStatusChanged();
    }

    update();
}

void ThumbnailItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_img.isNull()) {
        QRectF r = boundingRect();

        // TODO cache it
        QPixmap scaledPixmap = m_img.scaled(r.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QPointF centerPoint((r.width() - scaledPixmap.width()) / 2,
                            (r.height() - scaledPixmap.height())/ 2);

        painter->drawPixmap(centerPoint, scaledPixmap);
    }
}

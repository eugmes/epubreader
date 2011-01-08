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

#include "thumbnailerservice.h"
#include "thumbnailer_interface.h"
#include <QPixmap>
#include <QCryptographicHash>

ThumbnailerService::ThumbnailerService(QObject *parent) :
    QObject(parent)
{
    m_thumbnailer = new OrgFreedesktopThumbnailerGenericInterface(QLatin1String("org.freedesktop.thumbnailer"),
        QLatin1String("/org/freedesktop/thumbnailer/Generic"),
        QDBusConnection::sessionBus(), this);
    connect(m_thumbnailer, SIGNAL(Ready(QStringList)), SLOT(readyURLs(QStringList)));
}

static QPixmap loadThumbnailFile(const QString &url)
{
    QByteArray hash = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5);
    QString fileName = QDir::homePath() +
            QLatin1String("/.thumbnails/cropped/") +
            QLatin1String(hash.toHex()) + QLatin1String(".jpeg");
    qWarning() << "Thumbnail file name:" << fileName;
    return QPixmap(fileName);
}

void ThumbnailerService::getThumbnail(const QString &fileName)
{
    // Try to already created thumbnails and request only failed ones
    QString url = QUrl::fromLocalFile(fileName).toString();
    QPixmap pix = loadThumbnailFile(url);
    if (!pix.isNull()) {
        Q_EMIT thumbnailReady(fileName, pix);
        return;
    }

    m_thumbnailer->Queue(QStringList() << url, QStringList(), -1);
}

void ThumbnailerService::readyURLs(const QStringList &urls)
{
    Q_FOREACH (const QString &url, urls) {
        qWarning() << "readyFile" << QUrl(url).toLocalFile();

        QPixmap pix = loadThumbnailFile(url);
        if (!pix.isNull())
            Q_EMIT thumbnailReady(QUrl(url).toLocalFile(), pix);
    }
}

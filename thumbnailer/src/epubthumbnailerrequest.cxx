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

#include "epubthumbnailerrequest.h"
#include <QUrl>
#include <QXmlQuery>
#include <QImage>
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include "epubfile.h"

EPUBThumbnailerRequest::EPUBThumbnailerRequest(uint handle, const QStringList &uris, QObject *parent) :
    QThread(parent), m_handle(handle), m_uris(uris)
{
    connect(this, SIGNAL(finished()), SLOT(deleteLater()));
}

uint EPUBThumbnailerRequest::handle() const
{
    return m_handle;
}

void EPUBThumbnailerRequest::run()
{
    Q_FOREACH (const QString &uri, m_uris) {
        handleUri(uri);
    }

    Q_EMIT finished(m_handle);
    quit();
}

#define EPUB_NS_DECL "declare default element namespace \"http://www.idpf.org/2007/opf\";\n"

static QString getCoverID(const QString &metadata)
{
    QXmlQuery query;
    query.setFocus(metadata);
    query.setQuery(QLatin1String(EPUB_NS_DECL "string(metadata/meta[@name=\"cover\"]/@content)"));
    QStringList result;
    bool ret = query.evaluateTo(&result);
    if (!ret || !result.length())
        return QString();
    return result.at(0);
}

static const char *imageTypeForMimeType(const QString &mimeType)
{
    if (mimeType == QLatin1String("image/png"))
        return "PNG";
    else if (mimeType == QLatin1String("image/jpeg"))
        return "JPEG";
    return 0;
}

static bool saveThumbnail(const QString &uri, const QImage &img)
{
    QByteArray hash = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5);
    QString fileName = QDir::homePath() +
            QLatin1String("/.thumbnails/cropped/") +
            QLatin1String(hash.toHex()) + QLatin1String(".jpeg");
    return img.save(fileName, "JPEG");
}

void EPUBThumbnailerRequest::handleUri(const QString &uri)
{
    QString fileName = QUrl(uri).toLocalFile();
    if (fileName.isEmpty()) {
        Q_EMIT error(uri, 0, QLatin1String("Not a local URI"));
        qWarning() << "Not a local URI";
        return;
    }

    EPUBFile file(fileName);

    if (file.status() != EPUBFile::NoError) {
        Q_EMIT error(uri, file.status(), QLatin1String("File open error"));
        qWarning() << "File open error";
        return;
    }

    QString metadata = file.metadata();
    if (metadata.isEmpty()) {
        Q_EMIT error(uri, 0, QLatin1String("Empty metadata"));
        qWarning() << "Empty metadata";
        return;
    }

    QString coverID = getCoverID(metadata);
    if (coverID.isEmpty()) {
        Q_EMIT error(uri, 0, QLatin1String("Empty or absent cover id"));
        qWarning() << "Empty or absent cover id";
        return;
    }

    QUrl url = file.getUrlByID(coverID);
    if (!url.isValid()) {
        Q_EMIT error(uri, 0, QLatin1String("Cover path not found"));
        qWarning() << "Cover path not found";
        return;
    }

    QString mimeType;
    QByteArray coverImage = file.getFileByUrl(url, &mimeType);

    if (coverImage.isEmpty()) {
        Q_EMIT error(uri, 0, QLatin1String("Cover file is empty"));
        qWarning() << "Cover file is empty";
        return;
    }

    QImage img;
    img.loadFromData(coverImage, imageTypeForMimeType(mimeType));
    QImage scaled = img.scaled(QSize(124, 124), Qt::KeepAspectRatio, Qt::SmoothTransformation); // "cropped" image size

    if (!saveThumbnail(uri, scaled)) {
        Q_EMIT error(uri, 0, QLatin1String("Save failed"));
        qWarning() << "Save failed";
        return;
    }

    Q_EMIT ready(uri);
}

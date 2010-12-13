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

#include "epubfile.h"
#include "zipreader.h"
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QStringList>
#include <QRegExp>
#include <QDebug>

EPUBFile::EPUBFile(const QString &fileName, QObject *parent) :
    QObject(parent), m_zip(new ZipReader(fileName))
{
    QByteArray mimeType = m_zip->fileData("mimetype");
    if (mimeType.isNull()) {
        m_status = ZipError;
        return;
    }

    if (mimeType != "application/epub+zip") {
        m_status = MimeTypeError;
        return;
    }

    QByteArray container = m_zip->fileData("META-INF/container.xml");
    if (container.isEmpty()) {
        m_status = ContentError;
        return;
    }

    QXmlQuery query;
    query.setFocus(QString::fromUtf8(container));
    query.setQuery(
         "declare default element namespace \"urn:oasis:names:tc:opendocument:xmlns:container\";\n"
         "string(container/rootfiles/rootfile[@media-type=\"application/oebps-package+xml\"]/@full-path)");
    QXmlResultItems result;
    query.evaluateTo(&result);
    QXmlItem item(result.next());
    if (item.isNull()) {
        qWarning() << "content file name was not found.";
        m_status = ContentError;
        return;
    }

    QString contentFileName = item.toAtomicValue().toString();
    QRegExp rx("^/?(.*/)([^/]*)?$");
    if (rx.exactMatch(contentFileName))
        m_contentPrefix = rx.cap(1);

    m_status = NoError;
    parseContentFile(contentFileName);
}

EPUBFile::Status EPUBFile::status() const
{
    return m_status;
}

QString EPUBFile::metadata() const
{
    return m_metadata;
}

#define EPUB_NS_DECL "declare default element namespace \"http://www.idpf.org/2007/opf\";\n"

void EPUBFile::parseContentFile(const QString &fileName)
{
    Q_ASSERT(m_zip);
    QByteArray contentFile = m_zip->fileData(fileName);
    if (contentFile.isEmpty()) {
        m_status = ContentError;
        return;
    }

    QXmlQuery query;
    query.setFocus(QString::fromUtf8(contentFile));

    // get metadata
    query.setQuery(EPUB_NS_DECL "package/metadata");
    QString result;
    bool ret = query.evaluateTo(&result);
    if (!ret || result.isEmpty()) {
        qWarning() << "missing metadata";
        m_status = ContentError;
        return;
    }

    m_metadata = result;

    // get manifest
    query.setQuery(EPUB_NS_DECL "package/manifest/item");
    QXmlResultItems manifestResult;
    query.evaluateTo(&manifestResult);
    if (!parseManifest(query, manifestResult))
        return;

    // get spine
    query.setQuery(EPUB_NS_DECL "string(package/spine/@toc)");
    QXmlResultItems tocResult;
    query.evaluateTo(&tocResult);
    QXmlItem toc = tocResult.next();
    if (!toc.isAtomicValue()) {
        qWarning() << "cannot read toc name";
        m_status = ContentError;
        return;
    }
    query.setQuery(EPUB_NS_DECL "package/spine/itemref");
    QXmlResultItems spineResult;
    query.evaluateTo(&spineResult);
    if (!parseSpine(query, spineResult))
        return;
    // TODO process guide if present
}

bool EPUBFile::parseManifest(const QXmlQuery &parentQuery, QXmlResultItems &items)
{
    QXmlQuery idQuery(parentQuery), hrefQuery(parentQuery), mediaTypeQuery(parentQuery);

    QXmlItem item(items.next());

    while (!item.isNull()) {
        idQuery.setFocus(item);
        hrefQuery.setFocus(item);
        mediaTypeQuery.setFocus(item);

        idQuery.setQuery(EPUB_NS_DECL "string(@id)");
        hrefQuery.setQuery(EPUB_NS_DECL "string(@href)");
        mediaTypeQuery.setQuery(EPUB_NS_DECL "string(@media-type)");

        // TODO add fallback

        QXmlResultItems idResult;
        QXmlResultItems hrefResult;
        QXmlResultItems mediaTypeResult;

        idQuery.evaluateTo(&idResult);
        hrefQuery.evaluateTo(&hrefResult);
        mediaTypeQuery.evaluateTo(&mediaTypeResult);

        QXmlItem id = idResult.next();
        QXmlItem href = hrefResult.next();
        QXmlItem mediaType = mediaTypeResult.next();

        if (!(id.isAtomicValue() && href.isAtomicValue() && mediaType.isAtomicValue())) {
            qWarning() << "manifest parsing error";
            m_status = ContentError;
            return false;
        }

        m_manifest.append(ManifestItem(
                        id.toAtomicValue().toString(),
                        href.toAtomicValue().toString(),
                        mediaType.toAtomicValue().toString()));

        item = items.next();
    }
    return true;
}

QByteArray EPUBFile::getFileByPath(const QString &path, QString *mimeType)
{
    Q_ASSERT(mimeType);
    // TODO maybe handle fallback
    foreach (const ManifestItem &i, m_manifest) {
        if (i.href == path) {
            *mimeType = i.mediaType;
            return m_zip->fileData(m_contentPrefix + path);
        }
    }
    return QByteArray();
}

QString EPUBFile::getFilePathByID(const QString &id) const
{
    foreach (const ManifestItem &i, m_manifest) {
        if (i.id == id)
            return i.href;
    }
    return QString();
}

QString EPUBFile::getIDByPath(const QString &path) const
{
    foreach (const ManifestItem &i, m_manifest) {
        if (i.href == path)
            return i.id;
    }
    return QString();
}

bool EPUBFile::parseSpine(const QXmlQuery &parentQuery, QXmlResultItems &items)
{
    QXmlQuery idrefQuery(parentQuery);
    // TODO other attributes
    QXmlItem item(items.next());

    while (!item.isNull()) {
        idrefQuery.setFocus(item);
        idrefQuery.setQuery(EPUB_NS_DECL "string(@idref)");
        QXmlResultItems idrefResult;
        idrefQuery.evaluateTo(&idrefResult);
        QXmlItem idref = idrefResult.next();
        if (!idref.isAtomicValue()) {
            qWarning() << "incorrect idref";
            m_status = ContentError;
            return false;
        }
        m_spine.append(SpineItem(idref.toAtomicValue().toString()));
        item = items.next();
    }
    return true;
}

QString EPUBFile::getDefaultID() const
{
    if (m_spine.length() > 0)
        return m_spine[0].idref;
    else
        return QString();
}

QString EPUBFile::getPrevPage(const QString &path) const
{
    QString id = getIDByPath(path);

    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine[i].idref == id) {
            if (i > 0) {
                QString nextId = m_spine[i-1].idref;
                return getFilePathByID(nextId);
            } else {
                return path;
            }
        }
    }
    return path;
}

QString EPUBFile::getNextPage(const QString &path) const
{
    QString id = getIDByPath(path);

    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine[i].idref == id) {
            if (i < m_spine.size() - 1) {
                QString nextId = m_spine[i+1].idref;
                return getFilePathByID(nextId);
            } else {
                return path;
            }
        }
    }
    return path;
}

EPUBFile::PageInfo EPUBFile::getPathInfo(const QString &path) const
{
    QString id = getIDByPath(path);
    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine[i].idref == id) {
            return PageInfo(i > 0, i < m_spine.size() - 1);
        }
    }
    return PageInfo(false, false);
}

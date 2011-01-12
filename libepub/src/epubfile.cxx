/* Copyright © 2010-2011 Євгеній Мещеряков <eugen@debian.org>
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
#include <QBuffer>
#include <QDebug>

EPUBFile::EPUBFile(const QString &fileName, QObject *parent) :
    QObject(parent), m_zip(new ZipReader(fileName))
{
    QByteArray mimeType = m_zip->fileData(QLatin1String("mimetype"));
    if (mimeType.isNull()) {
        m_status = ZipError;
        return;
    }

    if (mimeType != "application/epub+zip") {
        m_status = MimeTypeError;
        return;
    }

    QByteArray container = m_zip->fileData(QLatin1String("META-INF/container.xml"));
    if (container.isEmpty()) {
        m_status = ContentError;
        return;
    }

    QBuffer buf(&container);
    buf.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.setFocus(&buf);

    query.setQuery(QLatin1String(
         "declare default element namespace \"urn:oasis:names:tc:opendocument:xmlns:container\";\n"
         "string(container/rootfiles/rootfile[@media-type=\"application/oebps-package+xml\"]/@full-path)"));
    QXmlResultItems result;
    query.evaluateTo(&result);
    QXmlItem item(result.next());
    if (item.isNull()) {
        qWarning() << "content file name was not found.";
        m_status = ContentError;
        return;
    }

    /* FIXME check spec: could contentFileName contain leading slash? */
    QString contentFileName = item.toAtomicValue().toString();
    m_contentBase = QUrl::fromEncoded("epub:///").resolved(QUrl(contentFileName)).resolved(QUrl::fromEncoded("."));

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

    QBuffer buf(&contentFile);
    buf.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.setFocus(&buf);

    // get metadata
    query.setQuery(QLatin1String(EPUB_NS_DECL "package/metadata"));
    QString result;
    bool ret = query.evaluateTo(&result);
    if (!ret || result.isEmpty()) {
        qWarning() << "missing metadata";
        m_status = ContentError;
        return;
    }

    m_metadata = result;

    // get manifest
    query.setQuery(QLatin1String(EPUB_NS_DECL "package/manifest/item"));
    QXmlResultItems manifestResult;
    query.evaluateTo(&manifestResult);
    if (!parseManifest(query, manifestResult))
        return;

    // get spine
    query.setQuery(QLatin1String(EPUB_NS_DECL "string(package/spine/@toc)"));
    QXmlResultItems tocResult;
    query.evaluateTo(&tocResult);
    QXmlItem toc = tocResult.next();
    if (!toc.isAtomicValue()) {
        qWarning() << "cannot read toc name";
        m_status = ContentError;
        return;
    }
    m_tocName = toc.toAtomicValue().toString();

    query.setQuery(QLatin1String(EPUB_NS_DECL "package/spine/itemref"));
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

        idQuery.setQuery(QLatin1String(EPUB_NS_DECL "string(@id)"));
        hrefQuery.setQuery(QLatin1String(EPUB_NS_DECL "string(@href)"));
        mediaTypeQuery.setQuery(QLatin1String(EPUB_NS_DECL "string(@media-type)"));

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
                        m_contentBase.resolved(QUrl(href.toAtomicValue().toString())),
                        mediaType.toAtomicValue().toString()));

        item = items.next();
    }
    return true;
}

QByteArray EPUBFile::getFileByUrl(QUrl url, QString *mimeType) const
{
    Q_ASSERT(mimeType);
    Q_ASSERT(url.scheme() == QLatin1String("epub"));
    Q_ASSERT(url.host().isEmpty());

    url.setFragment(QString());

    // TODO maybe handle fallback
    Q_FOREACH (const ManifestItem &i, m_manifest) {
        if (i.href == url) {
            *mimeType = i.mediaType;
            QString path = url.path();
            Q_ASSERT(path.startsWith(QLatin1Char('/')));
            return m_zip->fileData(path.mid(1));
        }
    }
    return QByteArray();
}

QUrl EPUBFile::getUrlByID(const QString &id) const
{
    Q_FOREACH (const ManifestItem &i, m_manifest) {
        if (i.id == id)
            return i.href;
    }
    return QUrl();
}

QString EPUBFile::getIDByUrl(QUrl url) const
{
    url.setFragment(QString());

    Q_FOREACH (const ManifestItem &i, m_manifest) {
        if (i.href == url)
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
        idrefQuery.setQuery(QLatin1String(EPUB_NS_DECL "string(@idref)"));
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

QUrl EPUBFile::defaultUrl() const
{
    if (m_spine.length() > 0)
        return getUrlByID(m_spine.at(0).idref);
    else
        return QUrl();
}

QUrl EPUBFile::getPrevPage(const QUrl &url) const
{
    QString id = getIDByUrl(url);

    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine.at(i).idref == id) {
            if (i > 0) {
                QString nextId = m_spine.at(i-1).idref;
                return getUrlByID(nextId);
            } else {
                return url;
            }
        }
    }
    return url;
}

QUrl EPUBFile::getNextPage(const QUrl &url) const
{
    QString id = getIDByUrl(url);

    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine.at(i).idref == id) {
            if (i < m_spine.size() - 1) {
                QString nextId = m_spine.at(i+1).idref;
                return getUrlByID(nextId);
            } else {
                return url;
            }
        }
    }
    return url;
}

EPUBFile::PageFlags EPUBFile::getUrlInfo(const QUrl &url) const
{
    QString id = getIDByUrl(url);
    for (int i = 0; i < m_spine.size(); i++) {
        if (m_spine.at(i).idref == id) {
            PageFlags flags = 0;
            if (i > 0)
                flags |= PageHasPrevious;
            if (i < m_spine.size() - 1)
                flags |= PageHasNext;
            return flags;
        }
    }
    return 0;
}

QUrl EPUBFile::resolveTocUrl(const QUrl &url) const
{
    return getUrlByID(m_tocName).resolved(url);
}

QByteArray EPUBFile::tocDocument() const
{
    QUrl tocUrl = getUrlByID(m_tocName);

    QString mimeType;
    QByteArray fileContent = getFileByUrl(tocUrl, &mimeType);
    if (mimeType != QLatin1String("application/x-dtbncx+xml"))
        return QByteArray();

    return fileContent;
}

bool EPUBFile::hasUrl(QUrl url) const
{
    url.setFragment(QString());
    Q_FOREACH (const ManifestItem &i, m_manifest) {
        if (i.href == url)
            return true;
    }
    return false;
}

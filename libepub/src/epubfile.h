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

#ifndef EPUBFILE_H
#define EPUBFILE_H
#include <QObject>
#include <QString>
#include <QList>
#include <QScopedPointer>
#include "zipreader.h"

class QXmlResultItems;
class QXmlQuery;

class EPUBFile : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString metadata READ metadata)
    Q_PROPERTY(QString tocPrefix READ tocPrefix)
public:
    explicit EPUBFile(const QString &fileName, QObject *parent = 0);

    enum Status {
        NoError,
        ZipError,
        MimeTypeError,
        ContentError
    };

    Status status() const;
    QString metadata() const;

    QByteArray getFileByPath(const QString &path, QString *mimeType);
    QString getFilePathByID(const QString &id) const;
    QString getIDByPath(const QString &id) const;
    QString getDefaultID() const;
    QString getPrevPage(const QString &path) const;
    QString getNextPage(const QString &path) const;

    struct PageInfo {
        PageInfo(bool prev, bool next) : hasPrev(prev), hasNext(next) {}
        bool hasPrev;
        bool hasNext;
    };

    PageInfo getPathInfo(const QString &path) const;

    QByteArray tocDocument();
    QString tocPrefix();

private:
    void parseContentFile(const QString &fileName);
    bool parseManifest(const QXmlQuery &parentQuery, QXmlResultItems &items);
    bool parseSpine(const QXmlQuery &parentQuery, QXmlResultItems &items);

    QScopedPointer<ZipReader> m_zip;
    Status m_status;

    class ManifestItem {
    public:
        ManifestItem(const QString &a_id, const QString &a_href, const QString &a_mediaType) :
            id(a_id), href(a_href), mediaType(a_mediaType) {}

        QString id;
        QString href;
        QString mediaType;
    };

    class SpineItem {
    public:
        SpineItem(const QString &a_idref) :
            idref(a_idref) {}

        QString idref;
    };

    QString m_metadata;
    QList<ManifestItem> m_manifest;
    QList<SpineItem> m_spine;

    QString m_contentPrefix;

    QString m_tocName;
};

#endif

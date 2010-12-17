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

#ifndef EPUBMETADATAPARSER_H
#define EPUBMETADATAPARSER_H

#include "epubabstractxmlparser.h"
#include <QHash>

class QXmlQuery;

class EPUBMetadataParser : public EPUBAbstractXMLParser {
public:
    EPUBMetadataParser(QXmlQuery *query);

    virtual void attribute(const QXmlName &name, const QStringRef &value);
    virtual void characters(const QStringRef &value);
    virtual void startElement(const QXmlName &name);
    virtual void endElement();

    typedef QPair<QString, QString> MetadataEntry;

    QList<MetadataEntry> metadata() const;

private:
    enum Status {
        Initial,
        InElemenet
    };

    QXmlQuery *m_query;
    Status m_status;
    int m_level;

    QString m_name;
    QString m_data;

    QHash<QString, QString> m_names;

    QList<MetadataEntry> m_metadata;
};

#endif

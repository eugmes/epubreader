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

#include "epubmetadataparser.h"
#include <QXmlQuery>
#include <QDebug>

EPUBMetadataParser::EPUBMetadataParser(QXmlQuery *query)
    : EPUBAbstractXMLParser(), m_query(query),
      m_status(EPUBMetadataParser::Initial), m_level(0)
{
#define MAP(attr, val) m_names[QLatin1String(attr)] = QLatin1String(val)
    MAP("contributro", "DC:Contributor");
    MAP("coverage", "DC:Coverage");
    MAP("creator", "DC:Creator");
    MAP("date", "DC:Date");
    MAP("description", "DC:Description");
    MAP("format", "DC:Format");
    MAP("identifier", "DC:Identifier");
    MAP("keywords", "DC:Keywords");
    MAP("language", "DC:Language");
    MAP("publisher", "DC:Publishers"); // FIXME array?
    MAP("relation", "DC:Relation");
    MAP("rights", "DC:Rights");
    MAP("source", "DC:Source");
    MAP("subject", "DC:Subject");
    MAP("title", "DC:Title");
    MAP("type", "DC:Type");
#undef MAP
}

void EPUBMetadataParser::attribute(const QXmlName &name, const QStringRef &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
}

void EPUBMetadataParser::characters(const QStringRef &value)
{
    m_data = value.toString();
}

void EPUBMetadataParser::startElement(const QXmlName &name)
{
    m_status = InElemenet;
    m_level++;

    if (name.namespaceUri(m_query->namePool()) == QLatin1String("http://purl.org/dc/elements/1.1/")) {
        QString nm = name.localName(m_query->namePool());
        m_name = m_names.value(nm);
    }
}

void EPUBMetadataParser::endElement()
{
    if (!--m_level) {
        m_status = Initial;

        m_metadata.append(MetadataEntry(m_name, m_data));

        m_name = QString();
        m_data = QString();
    }
}

QList<EPUBMetadataParser::MetadataEntry> EPUBMetadataParser::metadata() const
{
    return m_metadata;
}

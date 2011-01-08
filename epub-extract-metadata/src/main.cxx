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

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QXmlQuery>
#include <iostream>
#include "epubfile.h"
#include "epubmetadataparser.h"

#define DC_QUERY \
    "declare default element namespace \"http://www.idpf.org/2007/opf\";\n" \
    "declare namespace dc = \"http://purl.org/dc/elements/1.1/\";\n" \
    "metadata/dc:*"

void parseMetadata(const QString &s)
{
    QXmlQuery query;
    query.setFocus(s);
    query.setQuery(QLatin1String(DC_QUERY));

    EPUBMetadataParser parser(&query);

    query.evaluateTo(&parser);
    Q_FOREACH (const EPUBMetadataParser::MetadataEntry &ent, parser.metadata()) {
        std::cout << qPrintable(ent.first) << "\n";
        QString val = ent.second;
        std::cout << qPrintable(val.replace(QLatin1Char('\\'), QLatin1String("\\\\")).
                                replace(QLatin1Char('\n'), QLatin1String("\\n"))) << "\n";
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (app.arguments().count() < 2) {
        std::cerr << "Usage: " << qPrintable(app.arguments().at(0)) << " <file.epub>\n";
        return 1;
    }

    EPUBFile file(app.arguments().at(1));
    if (file.status() != EPUBFile::NoError)
        return 2;

    parseMetadata(file.metadata());

    return 0;
}

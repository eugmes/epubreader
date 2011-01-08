/* Copyright © 2011 Євгеній Мещеряков <eugen@debian.org>
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

#include "epubtocmodel.h"
#include <QBuffer>
#include <QXmlResultItems>
#include <QXmlQuery>
#include <QStringList>
#include <QDebug>

#define NCX_NS_DECL "declare default element namespace \"http://www.daisy.org/z3986/2005/ncx/\";"

EPUBTocModel::EPUBTocModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[UrlRole] = "contentUrl";
    setRoleNames(roles);
}

void EPUBTocModel::setDocument(const QByteArray &doc)
{
    QBuffer docBuffer;
    docBuffer.setData(doc);
    docBuffer.open(QIODevice::ReadOnly);

    beginResetModel();
    m_data.clear();

    QXmlQuery query;
    query.setFocus(&docBuffer);
    query.setQuery(QLatin1String(NCX_NS_DECL "ncx/navMap//navPoint"));
    QXmlResultItems result;
    query.evaluateTo(&result);
    QXmlItem item(result.next());
    while (!item.isNull()) {
        Record rec;
        QXmlQuery q1(query);
        q1.setFocus(item);
        QStringList l1;
        q1.setQuery(QLatin1String(NCX_NS_DECL "navLabel/text/string()"));
        if (q1.evaluateTo(&l1) && (l1.length() > 0))
            rec.header = l1.at(0);

        QXmlQuery q2(query);
        q2.setFocus(item);
        QStringList l2;
        q2.setQuery(QLatin1String(NCX_NS_DECL "content/@src/string()"));
        if (q2.evaluateTo(&l2) && (l2.length() > 0))
            rec.url = l2.at(0);

        m_data << rec;

        item = result.next();
    }

    endResetModel();

    Q_EMIT documentChanged();
}

int EPUBTocModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.count();
}

QVariant EPUBTocModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid())
        return QVariant();

    if (index.column() != 0)
        return QVariant();

    int row = index.row();

    if (row >= m_data.length()) // FIXME check row < 0?
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_data.at(row).header;
    case UrlRole:
        return m_data.at(row).url;
    default:
        break;
    }
    return QVariant();
}

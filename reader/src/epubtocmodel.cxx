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

#define NCX_NS_DECL "declare default element namespace \"http://www.daisy.org/z3986/2005/ncx/\";"

EPUBTocModel::EPUBTocModel(QObject *parent) :
    QAbstractListModel(parent), m_rowCount(0)
{
    m_docBuffer = new QBuffer(this);
    m_docBuffer->open(QIODevice::ReadOnly);

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[UrlRole] = "contentUrl";
    setRoleNames(roles);
}

QByteArray EPUBTocModel::document() const
{
    return m_docBuffer->data();
}

void EPUBTocModel::setDocument(const QByteArray &doc)
{
    m_docBuffer->close();
    m_docBuffer->setData(doc);
    m_docBuffer->open(QIODevice::ReadOnly);

    beginResetModel();
    m_query.setFocus(m_docBuffer);
    m_query.setQuery(QLatin1String(NCX_NS_DECL "count(/ncx/navMap//navPoint)"));
    QXmlResultItems result;
    m_query.evaluateTo(&result);
    QXmlItem item(result.next());
    if (item.isAtomicValue())
        m_rowCount = item.toAtomicValue().toInt();
    else
        m_rowCount = 0;
    endResetModel();

    emit documentChanged();
}

int EPUBTocModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_rowCount;
}

QVariant EPUBTocModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid())
        return QVariant();

    if (index.column() != 0)
        return QVariant();

    int row = index.row();

    switch (role) {
    case Qt::DisplayRole:
        return getData(QLatin1String("navLabel/text/string()"), row);
    case UrlRole:
        return getData(QLatin1String("content/@src/string()"), row);
    default:
        break;
    }
    return QVariant();
}

QVariant EPUBTocModel::getData(const QString &name, int index) const
{
    QString queryTpl(QLatin1String(NCX_NS_DECL "(/ncx/navMap//navPoint)[%1]/%2"));
    QString queryStr = queryTpl.arg(index + 1).arg(name);

    QXmlQuery query(m_query);
    query.setQuery(queryStr);
    QXmlResultItems result;
    query.evaluateTo(&result);
    QXmlItem item(result.next());
    return item.toAtomicValue();
}

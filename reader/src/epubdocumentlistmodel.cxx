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

#include "epubdocumentlistmodel.h"

EPUBDocumentListModel::EPUBDocumentListModel(const QStringList &data, QObject *parent) :
    QAbstractListModel(parent), m_data(data)
{
}

int EPUBDocumentListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant EPUBDocumentListModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (!index.isValid())
        return QVariant();
    if (index.parent().isValid())
        return QVariant();
    if (index.column() != 0)
        return QVariant();
    if (index.row() >= m_data.size())
        return QVariant();
    return m_data[index.row()];
}

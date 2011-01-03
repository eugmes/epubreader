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

#ifndef EPUBTOCMODEL_H
#define EPUBTOCMODEL_H

#include <QAbstractListModel>

class QBuffer;

class EPUBTocModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(Roles)
public:
    explicit EPUBTocModel(QObject *parent = 0);

    void setDocument(const QByteArray &doc);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    enum Roles {
        UrlRole = Qt::UserRole + 1
    };

signals:
    void documentChanged();

private:
    struct Record {
        QString header;
        QString url;
    };

    QList<Record> m_data;
};

#endif

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

#ifndef EPUBDOCUMENTLISTMODEL_H
#define EPUBDOCUMENTLISTMODEL_H

#include <QAbstractListModel>
#include <QDBusPendingCallWatcher>
#include <QPixmap>

class OrgFreedesktopTrackerSearchInterface;
class QPixmap;
class ThumbnailerService;

class EPUBDocumentListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit EPUBDocumentListModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Roles {
        FileNameRole = Qt::UserRole + 1
    };

private:
    class EPUBDesc {
    public:
        EPUBDesc(const QString &fileName, const QString &title)
        {
            this->fileName = fileName;
            this->title = title;
        }

        QString fileName;
        QString title;
        QPixmap thumbnail;
    };

    OrgFreedesktopTrackerSearchInterface *m_search;
    ThumbnailerService *m_thumbnailer;

    QList<EPUBDesc> m_data;

private slots:
    void callFinished(QDBusPendingCallWatcher *call);
    void thumbnailReady(const QString &fileName, const QPixmap &img);
};

#endif

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
#include "search_interface.h"
#include "thumbnailerservice.h"
#include <QDebug>

#define EPUB_QUERY \
    "<rdfq:Condition>" \
      "<rdfq:equals>" \
        "<rdfq:Property name=\"File:Mime\"/>" \
        "<rdf:String>application/epub+zip</rdf:String>" \
      "</rdfq:equals>" \
    "</rdfq:Condition>"

EPUBDocumentListModel::EPUBDocumentListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_search = new OrgFreedesktopTrackerSearchInterface(QLatin1String("org.freedesktop.Tracker"),
        QLatin1String("/org/freedesktop/Tracker/Search"),
        QDBusConnection::sessionBus(), this);

    QDBusPendingReply<StringListList> reply = m_search->Query(-1, QLatin1String("Files"),
                                   QStringList() << QLatin1String("DC:Title"),
                                   QLatin1String(""),
                                   QStringList(),
                                   QLatin1String(EPUB_QUERY),
                                   false,
                                   QStringList(),
                                   false, 0, -1);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(callFinished(QDBusPendingCallWatcher*)));

    m_thumbnailer = new ThumbnailerService(this);

    QHash<int, QByteArray> roleNames;
    roleNames[Qt::DisplayRole] = "display";
    roleNames[Qt::DecorationRole] = "decoration";
    roleNames[FileNameRole] = "fileName";
    setRoleNames(roleNames);

#ifndef Q_WS_MAEMO_5
    // Add some dummy data
    m_data << EPUBDesc(QLatin1String("/nonexistent.epub"),
                       QLatin1String("Simple Book"));
#endif
}

int EPUBDocumentListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant EPUBDocumentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.parent().isValid())
        return QVariant();
    if (index.column() != 0)
        return QVariant();
    if (index.row() >= m_data.size())
        return QVariant();

    int idx = index.row();
    if (role == Qt::DisplayRole)
        return m_data[idx].title;
    else if (role == Qt::DecorationRole) {
        QPixmap img = m_data[idx].thumbnail;
        if (img.isNull())
            return QVariant();
        return img;
    } else if (role == FileNameRole)
        return m_data[idx].fileName;
    return QVariant();
}

void EPUBDocumentListModel::callFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<StringListList> reply = *call;

    if (reply.isError()) {
        qWarning() << "Query call error";
    } else {
        beginResetModel();
        m_data.clear();

        QStringList fileNames;
        StringListList list = reply.argumentAt<0>();
        foreach (const QStringList &l, list) {
            if (l.length() < 3) {
                qWarning() << "Wrong entry length";
                continue;
            }

            fileNames << l[0];
            m_data << EPUBDesc(l[0], l[2]);
        }
        endResetModel();
        m_thumbnailer->getThumbnails(fileNames);
    }

    call->deleteLater();
}

void EPUBDocumentListModel::thumbnailReady(const QString &fileName, const QPixmap &img)
{
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data[i].fileName == fileName) {
            m_data[i].thumbnail = img;
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}

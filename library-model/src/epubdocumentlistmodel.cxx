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
    search = new OrgFreedesktopTrackerSearchInterface(QLatin1String("org.freedesktop.Tracker"),
        QLatin1String("/org/freedesktop/Tracker/Search"),
        QDBusConnection::sessionBus(), this);

    QDBusPendingReply<StringListList> reply = search->Query(-1, QLatin1String("Files"),
                                   QStringList() << QLatin1String("DC:Title"),
                                   QLatin1String(""),
                                   QStringList(),
                                   QLatin1String(EPUB_QUERY),
                                   false,
                                   QStringList(),
                                   false, 0, -1);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(callFinished(QDBusPendingCallWatcher*)));

#ifndef Q_WS_MAEMO_5
    // Add some dummy data
    EPUBDesc desc;
    desc.fileName = QLatin1String("/nonexistent.epub");
    desc.title = QLatin1String("Simple Book");
    m_data << desc;
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
    return m_data[index.row()].title;
}

void EPUBDocumentListModel::callFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<StringListList> reply = *call;

    if (reply.isError()) {
        qWarning() << "Query call error";
    } else {
        beginResetModel();
        m_data.clear();

        StringListList list = reply.argumentAt<0>();
        foreach (const QStringList &l, list) {
            EPUBDesc desc;

            if (l.length() < 3) {
                qWarning() << "Wrong entry length";
                continue;
            }

            desc.fileName = l[0];
            desc.title = l[2];
            m_data << desc;
        }
        endResetModel();
    }

    call->deleteLater();
}

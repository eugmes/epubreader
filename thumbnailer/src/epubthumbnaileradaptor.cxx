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

#include "epubthumbnaileradaptor.h"
#include "epubthumbnailerrequestqueue.h"
#include <QStringList>

EPUBThumbnailerAdaptor::EPUBThumbnailerAdaptor(EPUBThumbnailerRequestQueue *queue) :
    QDBusAbstractAdaptor(queue), m_queue(queue)
{
    connect(queue, SIGNAL(started(uint)), SIGNAL(Started(uint)));
    connect(queue, SIGNAL(ready(QString)), SIGNAL(Ready(QString)));
    connect(queue, SIGNAL(finished(uint)), SIGNAL(Finished(uint)));
    connect(queue, SIGNAL(error(QString,int,QString)), SIGNAL(Error(QString,int,QString)));
}

EPUBThumbnailerAdaptor::~EPUBThumbnailerAdaptor()
{
}

void EPUBThumbnailerAdaptor::Create(const QString &uri, const QString &mime_hint)
{
    CreateMany(QStringList() << uri, mime_hint);
}

uint EPUBThumbnailerAdaptor::CreateMany(const QStringList &uris, const QString &mime_hint)
{
    Q_UNUSED(mime_hint); // FIXME maybe raise an error instead
    return m_queue->enqueue(uris);
}

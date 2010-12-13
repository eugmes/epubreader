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

#include "epubthumbnailerrequestqueue.h"
#include "epubthumbnailerrequest.h"

EPUBThumbnailerRequestQueue::EPUBThumbnailerRequestQueue(QObject *parent) :
    QObject(parent), m_requestCounter(0), m_busy(false)
{

}

uint EPUBThumbnailerRequestQueue::enqueue(const QStringList &uris)
{
    uint handle = m_requestCounter++;

    EPUBThumbnailerRequest *request = new EPUBThumbnailerRequest(handle, uris, this);

    m_requests.enqueue(request);
    if (!m_busy)
        processRequest();

    return handle;
}

void EPUBThumbnailerRequestQueue::processRequest()
{
    if (m_requests.isEmpty()) {
        m_busy = false;
        emit requestQueueEmpty();
        return;
    }

    EPUBThumbnailerRequest *req = m_requests.dequeue();
    m_busy = true;

    connect(req, SIGNAL(ready(QString)), SIGNAL(ready(QString)), Qt::QueuedConnection);
    connect(req, SIGNAL(error(QString,int,QString)), SIGNAL(error(QString,int,QString)), Qt::QueuedConnection);
    connect(req, SIGNAL(finished(uint)), SIGNAL(finished(uint)), Qt::QueuedConnection);

    connect(req, SIGNAL(finished(uint)), SLOT(processRequest()), Qt::QueuedConnection);

    emit started(req->handle());
    req->start();
}

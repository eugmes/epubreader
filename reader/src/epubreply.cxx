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

#include "epubreply.h"
#include <QNetworkAccessManager>
#include <QTimer>
#include <QDebug>

EPUBReply::EPUBReply(EPUBFile *epub, QNetworkAccessManager::Operation op, const QNetworkRequest &req, QObject *parent) :
    QNetworkReply(parent), m_epub(epub), m_position(0)
{
    setOperation(op);
    setRequest(req);
    QTimer::singleShot(0, this, SLOT(processRequest()));
    qDebug() << "Loading" << req.url().toString();
}

void EPUBReply::processRequest()
{
    if (!m_epub) {
        setError(ConnectionRefusedError, tr("File is not opened"));
        Q_EMIT error(ConnectionRefusedError);
        Q_EMIT finished();
        return;
    }

    if (operation() != QNetworkAccessManager::GetOperation) {
        setError(ProtocolInvalidOperationError, tr("Operation is not supported"));
        Q_EMIT error(ProtocolInvalidOperationError);
        Q_EMIT finished();
        return;
    }

    QUrl url = request().url();

    if (url.scheme() != QLatin1String("epub")) {
        setError(ProtocolUnknownError, tr("Unsupported scheme"));
        Q_EMIT error(ProtocolUnknownError);
        Q_EMIT finished();
        return;
    }

    QString mimeType;
    QString path = url.path();
    if (path.startsWith(QLatin1Char('/')))
        path = path.mid(1);
    m_content = m_epub->getFileByPath(path, &mimeType);

    if (m_content.isNull()) {
        setError(ContentNotFoundError, tr("File not found"));
        Q_EMIT error(ContentNotFoundError);
        Q_EMIT finished();
        return;
    }

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    setHeader(QNetworkRequest::ContentLengthHeader, m_content.size());

    Q_EMIT readyRead();
    Q_EMIT finished();
}

void EPUBReply::abort()
{
}

bool EPUBReply::isSequential() const
{
    return true;
}

qint64 EPUBReply::bytesAvailable() const
{
    return m_content.size() - m_position;
}

qint64 EPUBReply::readData(char *data, qint64 maxlen)
{
    if (m_position < m_content.size()) {
        qint64 num = qMin(maxlen, m_content.size() - m_position);
        memcpy(data, m_content.constData() + m_position, num);
        m_position += num;
        return num;
    }
    return -1;
}

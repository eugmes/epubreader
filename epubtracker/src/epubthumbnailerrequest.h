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

#ifndef EPUBTHUMBNAILERREQUEST_H
#define EPUBTHUMBNAILERREQUEST_H

#include <QObject>
#include <QStringList>

class EPUBThumbnailerRequest : public QObject {
    Q_OBJECT
public:
    explicit EPUBThumbnailerRequest(uint handle, const QStringList &uris, QObject *parent = 0);

    void startProcessing();

signals:
    void finished(uint handle);
    void error(const QString &uri, int errorCode, const QString &message);

private:
    uint m_handle;
    QStringList m_uris;
    QThread *processingThread;
};

#endif

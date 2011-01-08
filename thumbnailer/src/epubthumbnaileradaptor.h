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

#ifndef EPUBTHUMBNAILERADAPTOR_H
#define EPUBTHUMBNAILERADAPTOR_H

#include <QDBusAbstractAdaptor>

class EPUBThumbnailerRequestQueue;

class EPUBThumbnailerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.thumbnailer.Thumbnailer")
public:
    explicit EPUBThumbnailerAdaptor(EPUBThumbnailerRequestQueue *queue);
    virtual ~EPUBThumbnailerAdaptor();

Q_SIGNALS:
    //void Ready(uint handle, const QString &uri);
    //void Started(uint handle);
    //void Finished(uint handle);
    //void Error(uint handle, const QString &failed_uri, int error_code, const QString &message);
    void Ready(const QString &uri);
    void Started(uint handle);
    void Finished(uint handle);
    void Error(const QString &uri, int errorCode, const QString &message);

public Q_SLOTS:
    //uint Queue(const QString &uri, const QString &mime_type, const QString &flavor, bool urgent);
    //void Dequeue(uint handle);
    void Create(const QString &uri, const QString &mime_hint);
    uint CreateMany(const QStringList &uris, const QString &mime_hint);

private:
    EPUBThumbnailerRequestQueue *m_queue;
};

#endif

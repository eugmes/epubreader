/* Copyright © 2010-2011 Євгеній Мещеряков <eugen@debian.org>
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

#ifndef EPUBACCESSMANAGER_H
#define EPUBACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QPointer>
#include "epubfile.h"

class EPUBAccessManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    explicit EPUBAccessManager(QObject *parent = 0);
    void setDocument(EPUBFile *epub);
protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
private:
    QPointer<EPUBFile> m_epub;
};

#endif

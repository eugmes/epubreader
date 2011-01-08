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

#ifndef THUMBNAILERSERVICE_H
#define THUMBNAILERSERVICE_H

#include <QObject>

class OrgFreedesktopThumbnailerGenericInterface;
class QPixmap;
class QStringList;

class ThumbnailerService : public QObject
{
    Q_OBJECT
public:
    explicit ThumbnailerService(QObject *parent = 0);

    void getThumbnail(const QString &fileName);

Q_SIGNALS:
    void thumbnailReady(const QString &fileName, const QPixmap &img);

private Q_SLOTS:
    void readyURLs(const QStringList &urls);

private:
    OrgFreedesktopThumbnailerGenericInterface *m_thumbnailer;
};

#endif

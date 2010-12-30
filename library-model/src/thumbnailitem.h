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

#ifndef THUMBNAILITEM_H
#define THUMBNAILITEM_H

#include <QDeclarativeItem>

class ThumbnailerService;

class ThumbnailItem : public QDeclarativeItem {
    Q_OBJECT
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadStatusChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
public:
    explicit ThumbnailItem(QDeclarativeItem *parent = 0);

    bool loaded() const;

    QString file() const;
    void setFile(const QString &fileName);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

signals:
    void loadStatusChanged();
    void fileChanged();

private slots:
    void thumbnailReady(const QString &fileName, const QPixmap &img);

private:
    static ThumbnailerService *m_thumbnailer;

    void setImage(const QPixmap &img);

    bool m_loaded;
    QString m_fileName;
    QPixmap m_img;
};

#endif

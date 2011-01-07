/* Copyright © 2011 Євгеній Мещеряков <eugen@debian.org>
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

#include "hildonimageprovider.h"
#include <QIcon>
#include <QDebug>

HildonImageProvider::HildonImageProvider():
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}

QPixmap HildonImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_ASSERT(size);

    QIcon icon = QIcon::fromTheme(id);
    QPixmap pix = icon.pixmap(requestedSize);
    *size = pix.size();
    return pix;
}

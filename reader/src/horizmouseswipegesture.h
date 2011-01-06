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

#ifndef HORIZMOUSESWIPEGESTURE_H
#define HORIZMOUSESWIPEGESTURE_H

#include <QGesture>

class HorizMouseSwipeGesture : public QGesture
{
    Q_OBJECT
    friend class HorizMouseSwipeGestureRecognizer;
public:
    explicit HorizMouseSwipeGesture(QObject *parent = 0);

    bool left() const;
private:
    QPoint m_offset;
    QTime m_start;
    bool m_left;
};

#endif

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

#ifndef EPUBTOCWINDOW_H
#define EPUBTOCWINDOW_H

#include <QMainWindow>

class EPUBTOCWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(QByteArray document READ document CONSTANT)
public:
    explicit EPUBTOCWindow(const QByteArray &xml, QWidget *parent = 0);

    QByteArray document() const;

private:
    QByteArray m_doc;
};

#endif

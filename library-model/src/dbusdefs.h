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

#ifndef EPUB_LIBRARY_DBUSDEFS_H
#define EPUB_LIBRARY_DBUSDEFS_H

#include <QList>
#include <QStringList>
#include <QMetaType>
#include <QDBusArgument>
#include <QDBusMetaType>

typedef QList<QStringList> StringListList;

Q_DECLARE_METATYPE(StringListList)

static inline QDBusArgument &operator<<(QDBusArgument &argument, const StringListList &list) {
    argument.beginArray(qMetaTypeId<QStringList>());
    foreach (const QStringList &sub, list)
        argument << sub;
    argument.endArray();

    return argument;
}

static inline const QDBusArgument &operator>>(const QDBusArgument &argument, StringListList &list) {
    argument.beginArray();
    list.clear();

    while (!argument.atEnd()) {
        QStringList element;
        argument >> element;
        list.append(element);
    }

    argument.endArray();

    return argument;
}

static inline void registerLibraryMetatypes()
{
    qRegisterMetaType<StringListList>();
    qDBusRegisterMetaType<StringListList>();
}

#endif

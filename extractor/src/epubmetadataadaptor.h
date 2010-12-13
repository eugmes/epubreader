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

#ifndef EPUBMETADATAADAPTOR_H
#define EPUBMETADATAADAPTOR_H

#include <QDBusAbstractAdaptor>

class EPUBTrackerApplication;

class EPUBMetadataAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.opensource.epubreader.MetadataExtractor1")
public:
    explicit EPUBMetadataAdaptor(EPUBTrackerApplication *app);

signals:

public slots:

};

#endif

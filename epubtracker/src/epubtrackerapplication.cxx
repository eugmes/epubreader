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

#include "epubtrackerapplication.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include "epubthumbnaileradaptor.h"
#include "epubthumbnailerrequestqueue.h"

EPUBTrackerApplication::EPUBTrackerApplication(int argc, char **argv) :
    QCoreApplication(argc, argv)
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    EPUBThumbnailerRequestQueue *thumbQueue = new EPUBThumbnailerRequestQueue(this);
    new EPUBThumbnailerAdaptor(thumbQueue);

    sessionBus.registerObject("/org/opensource/epubreader/Thumbnailer1", thumbQueue);
    sessionBus.registerService("org.opensource.epubreader.Thumbnailer1");
}

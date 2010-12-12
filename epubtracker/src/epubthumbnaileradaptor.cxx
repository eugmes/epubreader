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

#include "epubthumbnaileradaptor.h"
#include "epubtrackerapplication.h"

EPUBThumbnailerAdaptor::EPUBThumbnailerAdaptor(EPUBTrackerApplication *app) :
    QDBusAbstractAdaptor(app)
{
}

uint EPUBThumbnailerAdaptor::Queue(const QString &uri, const QString &mime_type, const QString &flavor, bool urgent)
{
    // TODO
    return 0;
}

void EPUBThumbnailerAdaptor::Dequeue(uint handle)
{
    // TODO
}

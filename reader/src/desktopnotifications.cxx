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

#include "desktopnotifications.h"
#include <libnotify/notify.h>

bool DesktopNotifications::init(const QString &appName)
{
    return notify_init(appName.toUtf8());
}

void DesktopNotifications::showInfoprint(const QString &message)
{
    QString info = QLatin1String("Information"); // XXX

    NotifyNotification *notification = notify_notification_new(info.toUtf8(), message.toUtf8(), "dialog-information", 0);
    if (notification) {
        notify_notification_set_timeout(notification, -1);
        notify_notification_set_category(notification, "system.note.infoprint");
        notify_notification_show(notification, 0);
        g_object_unref(notification);
    }
}

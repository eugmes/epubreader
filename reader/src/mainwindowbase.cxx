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

#include "mainwindowbase.h"
#include "epubreaderapplication.h"

MainWindowBase::MainWindowBase(QWidget *parent) :
    QMainWindow(parent)
{
    EPUBReaderSettings *settings = EPUBReaderApplication::settings();

    connect(settings, SIGNAL(windowOrientationChanged()), SLOT(changeWindowOrientation()));
    changeWindowOrientation();

    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

void MainWindowBase::changeWindowOrientation()
{
    EPUBReaderSettings *settings = EPUBReaderApplication::settings();
    m_savedOrientation = settings->windowOrientation();
    updateWindowOrientation();
}

void MainWindowBase::updateWindowOrientation()
{
#ifdef Q_WS_MAEMO_5
    switch (m_savedOrientation) {
    case EPUBReaderSettings::LandscapeOrientation:
        setAttribute(Qt::WA_Maemo5LandscapeOrientation);
        break;
    case EPUBReaderSettings::PortraitOrientation:
        setAttribute(Qt::WA_Maemo5PortraitOrientation);
        break;
    default:
        setAttribute(Qt::WA_Maemo5AutoOrientation);
    }
#endif
}

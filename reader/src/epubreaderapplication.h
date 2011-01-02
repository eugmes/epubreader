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

#ifndef EPUBREADERAPPLICATION_H
#define EPUBREADERAPPLICATION_H

#include <QApplication>
#include <QDBusAbstractAdaptor>

class MainWindow;
class EPUBReaderSettings;

class EPUBReaderApplication : public QApplication
{
    Q_OBJECT
public:
    explicit EPUBReaderApplication(int &argc, char **argv);

    void openFile(const QString &fileName);
    static EPUBReaderSettings *settings();

public slots:
    void openNewWindow();

private:
    void showNewWindow(MainWindow *win);

    EPUBReaderSettings *m_settings;
};

class EPUBApplicationDBusAdapter : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.opensource.epubreader.Reader")
public:
    explicit EPUBApplicationDBusAdapter(EPUBReaderApplication *app);

public slots:
    int mime_open(const QString &s1);
};

#endif

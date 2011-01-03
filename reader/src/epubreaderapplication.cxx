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

#include "epubreaderapplication.h"
#include "epubview.h"
#include "mainwindow.h"
#include "dbusdefs.h"
#include <QDBusConnection>
#include <QtDeclarative>
#include <QDBusError>
#include "thumbnailitem.h"
#include "epubreadersettings.h"
#include "horizmouseswipegesturerecognizer.h"
#include "epubtocmodel.h"

EPUBApplicationDBusAdapter::EPUBApplicationDBusAdapter(EPUBReaderApplication *app) :
    QDBusAbstractAdaptor(app)
{
}

int EPUBApplicationDBusAdapter::mime_open(const QString &s1)
{
    EPUBReaderApplication *app = qobject_cast<EPUBReaderApplication *>(parent());
    Q_ASSERT(app);

    if (!s1.isEmpty()) {
        QUrl fileUrl(s1);
        QString fileName = fileUrl.toLocalFile();
        if (fileName.isEmpty())
            return 0; // FIXME make it possible to open URLs too
        app->openFile(fileName);
    }

    return 0;
}

EPUBReaderApplication::EPUBReaderApplication(int &argc, char**argv) :
    QApplication(argc, argv)
{
    setApplicationName(QLatin1String("EPUBReader"));

    m_swipeGestureType = QGestureRecognizer::registerRecognizer(new HorizMouseSwipeGestureRecognizer);

    m_settings = new EPUBReaderSettings(this);

    registerLibraryMetatypes();

    qmlRegisterType<EPUBView>("EPUBReader", 1, 0, "EPUBView");
    qmlRegisterType<ThumbnailItem>("EPUBReader", 1, 0, "ThumbnailItem");
    qmlRegisterType<EPUBTocModel>("EPUBReader", 1, 0, "EPUBTocModel");

    MainWindow *win = new MainWindow;

    // TODO better error handling
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "Cannot connect to session bus.";
        exit(1);
    }

    new EPUBApplicationDBusAdapter(this);

    if (!QDBusConnection::sessionBus().registerObject(QLatin1String("/org/opensource/epubreader/Reader"), this)) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(3);
    }

    if (!QDBusConnection::sessionBus().registerService(QLatin1String("org.opensource.epubreader.Reader"))) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(2);
    }

    showNewWindow(win);
}

void EPUBReaderApplication::openFile(const QString &fileName)
{
    /* First try to find an existing window without open file */
    foreach (QWidget *w, topLevelWidgets()) {
        MainWindow *win = qobject_cast<MainWindow *>(w);
        if (win && win->fileName().isEmpty()) {
            win->openFile(fileName);
            win->activateWindow();
            return;
        }
    }

    /* If an empty window was not found, create new one */
    MainWindow *win = new MainWindow;
    win->openFile(fileName);
    showNewWindow(win);
}

EPUBReaderSettings *EPUBReaderApplication::settings()
{
    return qobject_cast<EPUBReaderApplication *>(instance())->m_settings;
}

Qt::GestureType EPUBReaderApplication::swipeGestureType()
{
    return qobject_cast<EPUBReaderApplication *>(instance())->m_swipeGestureType;
}

void EPUBReaderApplication::openNewWindow()
{
    MainWindow *win = new MainWindow;
    showNewWindow(win);
}

void EPUBReaderApplication::showNewWindow(MainWindow *win)
{
    connect(win, SIGNAL(newWindow()), SLOT(openNewWindow()));

#ifdef Q_WS_MAEMO_5
    win->showMaximized();
#else
    win->setGeometry(100, 100, 800, 480);
    win->show();
#endif
}

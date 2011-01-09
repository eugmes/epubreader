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

#include "mainwindow.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "settingsdialog.h"
#include "epublibrarybrowser.h"
#include "epubreaderapplication.h"
#include "epubtocwindow.h"
#include "hildonimageprovider.h"

MainWindow::MainWindow(QWidget *parent) :
    MainWindowBase(parent), m_orientationOverride(false), m_showLibrary(false)
{
    setWindowTitle(tr("E-Book Reader"));

    QDeclarativeView *view = new QDeclarativeView;
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty(QLatin1String("mainWindow"), this);
    EPUBReaderSettings *settings = EPUBReaderApplication::settings();
    view->rootContext()->setContextProperty(QLatin1String("settings"), settings);
    view->engine()->addImageProvider(QLatin1String("hildon-icon"), new HildonImageProvider);
    view->setSource(QUrl(QLatin1String("qrc:/qml/epubreader.qml")));

    setCentralWidget(view);

    QAction *openAction = new QAction(tr("Open"), this);
    connect(openAction, SIGNAL(triggered()), SLOT(chooseFile()));

    QAction *newWindowAction = new QAction(tr("New Window"), this);
    connect(newWindowAction, SIGNAL(triggered()), SIGNAL(newWindow()));

    QAction *libraryAction = new QAction(tr("Library"), this);
    connect(libraryAction, SIGNAL(triggered()), SLOT(showLibrary()));

    QAction *settingsAction = new QAction(tr("Settings"), this);

    QMenuBar *menu = menuBar();
    menu->addAction(openAction);
    menu->addAction(newWindowAction);
    menu->addAction(libraryAction);
    menu->addAction(settingsAction);

    connect(settingsAction, SIGNAL(triggered()), SLOT(showSettingsDialog()));
}

void MainWindow::updateWindowOrientation()
{
    if (!m_orientationOverride)
        MainWindowBase::updateWindowOrientation();
}

void MainWindow::chooseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QLatin1String("/"), tr("EPUB Files (*.epub)"));
    if (!fileName.isNull())
        openFile(fileName);
}

void MainWindow::openFile(QString newFileName)
{
    if (newFileName != m_fileName) {
        m_fileName = newFileName;
        Q_EMIT fileNameChanged();
    }
}

QString MainWindow::fileName() const
{
    return m_fileName;
}

bool MainWindow::fullSize() const
{
    return windowState() & Qt::WindowFullScreen;
}

void MainWindow::setFullSize(bool set)
{
    bool isFullSize = fullSize();
    if (set != isFullSize) {
        if (set)
            showFullScreen();
        else
            showNormal();
        Q_EMIT fullSizeChanged();
    }
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog *dlg = new SettingsDialog(this);

    m_orientationOverride = true;
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5LandscapeOrientation);
#endif
    dlg->exec();
    m_orientationOverride = false;
    updateWindowOrientation();
    delete dlg;
}

void MainWindow::showLibrary()
{
    EPUBLibraryBrowser *win = new EPUBLibraryBrowser(this);
    win->setWindowModality(Qt::WindowModal);
    connect(win, SIGNAL(openFileRequest(QString)), SLOT(openFile(QString)));
    connect(this, SIGNAL(fileNameChanged()), win, SLOT(close()));

#ifdef Q_WS_MAEMO_5
    win->showMaximized();
#else
    win->setGeometry(100, 100, 800, 480);
    win->show();
#endif
}

void MainWindow::showToc(const QByteArray &tocDocument)
{
    EPUBTOCWindow *win = new EPUBTOCWindow(tocDocument, this);
    win->setWindowModality(Qt::WindowModal);
    connect(win, SIGNAL(openTocDocumentRequest(QString)), SIGNAL(openTocDocumentRequest(QString)));
    // FIXME should not happen after everything fixed
    connect(this, SIGNAL(fileNameChanged()), win, SLOT(close()));

#ifdef Q_WS_MAEMO_5
    win->showMaximized();
#else
    win->setGeometry(100, 100, 800, 480);
    win->show();
#endif
}

void MainWindow::setFirstWindow()
{
    EPUBReaderSettings *settings = EPUBReaderApplication::settings();
    QString fileName = settings->lastOpenFile();

    if (!fileName.isEmpty() && QFile::exists(fileName))
        openFile(fileName);
    else
        m_showLibrary = true;
}

void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if (m_showLibrary) {
        m_showLibrary = false;
        showLibrary();
    }
}

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

#include "epublibrarybrowser.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "epubdocumentlistmodel.h"
#include <QDebug>

EPUBLibraryBrowser::EPUBLibraryBrowser(QWidget *parent) :
    MainWindowBase(parent)
{
    setWindowTitle(tr("E-Book Library"));

    EPUBDocumentListModel *model = new EPUBDocumentListModel(this);

    QDeclarativeView *view = new QDeclarativeView;
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty(QLatin1String("documentListModel"), model);
    view->rootContext()->setContextProperty(QLatin1String("libraryBrowser"), this);
    view->setSource(QUrl(QLatin1String("qrc:/qml/epublibrary.qml")));

    setCentralWidget(view);
}

void EPUBLibraryBrowser::openFile(const QString &fileName)
{
    Q_EMIT openFileRequest(fileName);
    close();
}

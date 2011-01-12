/* Copyright © 2010-2011 Євгеній Мещеряков <eugen@debian.org>
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
#include "hildonimageprovider.h"
#include <QDeclarativeEngine>
#include <QDebug>
#include <QSortFilterProxyModel>

EPUBLibraryBrowser::EPUBLibraryBrowser(QWidget *parent) :
    MainWindowBase(parent)
{
    setWindowTitle(tr("E-Book Library"));

    EPUBDocumentListModel *model = new EPUBDocumentListModel(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setSortRole(Qt::DisplayRole);
    proxyModel->setSortLocaleAware(true);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->sort(0);

    QDeclarativeView *view = new QDeclarativeView;
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty(QLatin1String("documentListModel"), proxyModel);
    view->rootContext()->setContextProperty(QLatin1String("libraryBrowser"), this);
    view->engine()->addImageProvider(QLatin1String("hildon-icon"), new HildonImageProvider);
#ifdef Q_WS_MAEMO_5
    view->rootContext()->setContextProperty(QLatin1String("runOnMaemo"), QVariant(true));
#else
    view->rootContext()->setContextProperty(QLatin1String("runOnMaemo"), QVariant(false));
#endif
    view->setSource(QUrl(QLatin1String("qrc:/qml/epublibrary.qml")));

    setCentralWidget(view);
}

void EPUBLibraryBrowser::openFile(const QString &fileName)
{
    Q_EMIT openFileRequest(fileName);
    close();
}

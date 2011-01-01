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

#include "epubtocwindow.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>

EPUBTOCWindow::EPUBTOCWindow(const QByteArray &xml, QWidget *parent) :
    QMainWindow(parent), m_doc(xml)
{
    setWindowTitle(tr("Table of Content"));

    QDeclarativeView *view = new QDeclarativeView;
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty(QLatin1String("tocWindow"), this);
    view->setSource(QUrl(QLatin1String("qrc:/qml/tocbrowser.qml")));

    setCentralWidget(view);

    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation);
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

QByteArray EPUBTOCWindow::document() const
{
    return m_doc;
}

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
#include <QListView>
#include <QVBoxLayout>
#include "epubtocmodel.h"

EPUBTOCWindow::EPUBTOCWindow(const QByteArray &tocDocument, QWidget *parent) :
    MainWindowBase(parent)
{
    setWindowTitle(tr("Table of Content"));

    QWidget *w = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    QListView *view = new QListView;
    EPUBTocModel *model = new EPUBTocModel(this);
    model->setDocument(tocDocument);
    view->setModel(model);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(view, SIGNAL(activated(QModelIndex)), SLOT(itemActivated(QModelIndex)));

    vbox->addWidget(view);
    w->setLayout(vbox);
    setCentralWidget(w);
}

void EPUBTOCWindow::itemActivated(const QModelIndex &index)
{
    Q_EMIT openTocDocumentRequest(index.data(EPUBTocModel::UrlRole).toString());
    close();
}

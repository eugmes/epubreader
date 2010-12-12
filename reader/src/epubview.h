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

#ifndef EPUBVIEW_H
#define EPUBVIEW_H

#include <QGraphicsWebView>

class EPUBFile;

class EPUBView : public QGraphicsWebView {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE openFile)
    Q_PROPERTY(QAction* prevPage READ prevPageAction CONSTANT)
    Q_PROPERTY(QAction* nextPage READ nextPageAction CONSTANT)
    Q_PROPERTY(int preferredWidth READ preferredWidth WRITE setPreferredWidth NOTIFY preferredWidthChanged)
    Q_PROPERTY(int preferredHeight READ preferredHeight WRITE setPreferredHeight NOTIFY preferredHeightChanged)
    Q_PROPERTY(int defaultFontSize READ defaultFontSize WRITE setDefaultFontSize)
public:
    explicit EPUBView(QGraphicsItem *parent = 0);

    QString fileName() const;
    QAction *prevPageAction() const;
    QAction *nextPageAction() const;

    int preferredWidth() const;
    int preferredHeight() const;
    int defaultFontSize() const;

public slots:
    bool openFile(const QString &fileName);

    bool showPrevPage();
    bool showNextPage();

    void setPreferredWidth(int width);
    void setPreferredHeight(int height);
    void setDefaultFontSize(int size);

signals:
    void preferredHeightChanged();
    void preferredWidthChanged();

    void badFile();

private slots:
    void handleUrlChange(const QUrl &url);

private:
    void showPath(const QString &path);
    void resizeContent();

    EPUBFile *m_epub;
    QString m_fileName;

    QAction *m_prevPageAction;
    QAction *m_nextPageAction;


    int m_preferredWidth;
    int m_preferredHeight;
};

#endif

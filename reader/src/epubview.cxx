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

#include "epubview.h"
#include "epubaccessmanager.h"
#include "epubfile.h"
#include <QAction>
#include <QWebFrame>
#include <QDebug>

#define STYLESHEET_TEMPLATE \
    "html {" \
        "background: %1 !important;" \
        "margin-top: 0px !important" \
    "}" \
    "body {" \
        "background: transparent !important;" \
        "margin-top: 70px !important;" \
        "margin-bottom: 70px !important;" \
        "margin-right: 10px !important;" \
        "margin-left: 10px !important" \
    "}"

static inline QString trimPath(const QString &path)
{
    if (path.startsWith(QLatin1Char('/')))
        return path.mid(1);
    return path;
}

EPUBView::EPUBView(QGraphicsItem *parent) :
    QGraphicsWebView(parent), m_epub(0), m_preferredWidth(800), m_preferredHeight(600)
{
    QWebSettings *s = settings();

    s->setAttribute(QWebSettings::JavascriptEnabled, false);
    s->setAttribute(QWebSettings::JavaEnabled, false);
    s->setAttribute(QWebSettings::FrameFlatteningEnabled, true);

    QWebFrame *frame = page()->mainFrame();
    frame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    frame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    page()->setNetworkAccessManager(new EPUBAccessManager);

    m_prevPageAction = new QAction(this);
    m_nextPageAction = new QAction(this);

    connect(m_prevPageAction, SIGNAL(triggered()), SLOT(showPrevPage()));
    connect(m_nextPageAction, SIGNAL(triggered()), SLOT(showNextPage()));

    m_prevPageAction->setEnabled(false);
    m_nextPageAction->setEnabled(false);

    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(handleUrlChange(QUrl)));

    resizeContent();
    setResizesToContents(true);

    setBackgroundIndex(0);
}

int EPUBView::backgroundIndex() const
{
    return m_backgroundIndex;
}

void EPUBView::setBackgroundIndex(int idx)
{
    m_backgroundIndex = idx;
    QColor c;

    switch (m_backgroundIndex) {
    default:
        c = QColor(Qt::white);
        break;
    case 1:
        c = QColor(0xf1, 0xdc, 0x6b);
        break;
    case 2:
        c = QColor(Qt::gray);
        break;
    }

    qDebug() << "RGB" << c.name();
    QString style = QString::fromLatin1(STYLESHEET_TEMPLATE).arg(c.name());
    QByteArray userStyleSheet = style.toUtf8();
    QString url = QLatin1String("data:text/css;charset=utf-8;base64,") + QLatin1String(userStyleSheet.toBase64());
    settings()->setUserStyleSheetUrl(url);
}

bool EPUBView::openFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    EPUBAccessManager *manager = qobject_cast<EPUBAccessManager *>(page()->networkAccessManager());
    Q_ASSERT(manager);

    EPUBFile *newEPUB = new EPUBFile(fileName, this);
    if (newEPUB->status() == EPUBFile::NoError) {
        m_prevPageAction->setEnabled(false);
        m_nextPageAction->setEnabled(false);

        if (m_epub) {
            manager->setDocument(0);
            delete m_epub;
        }

        m_epub = newEPUB;

        manager->setDocument(m_epub);
        QString defaultPage = m_epub->getDefaultID();
        QString path = m_epub->getFilePathByID(defaultPage);
        m_fileName = fileName;
        showPath(path);
        return true;
    }

    delete newEPUB;

    //setContent("No file"); // TODO add some error page
    // TODO show an error page and exit
    return false;
}

QString EPUBView::fileName() const
{
    return m_fileName;
}

QAction *EPUBView::prevPageAction() const
{
    return m_prevPageAction;
}

QAction *EPUBView::nextPageAction() const
{
    return m_nextPageAction;
}

bool EPUBView::showPrevPage()
{
    if (url().scheme() != QLatin1String("epub"))
        return false; // TODO mayve just try to show first page
    if (!m_epub)
        return false;
    QString path = trimPath(url().path());
    QString newPath = m_epub->getPrevPage(path);
    showPath(newPath);
    return true;
}

bool EPUBView::showNextPage()
{
    if (url().scheme() != QLatin1String("epub"))
        return false; // TODO maybe try to show last page
    if (!m_epub)
        return false;
    QString path = trimPath(url().path());
    QString newPath = m_epub->getNextPage(path);
    showPath(newPath);
    return true;
}

void EPUBView::showPath(const QString &path)
{
    setUrl(QUrl(QLatin1String("epub:/") + path));
}

void EPUBView::handleUrlChange(const QUrl &url)
{
    if (url.scheme() != QLatin1String("epub")) {
        m_prevPageAction->setEnabled(false);
        m_nextPageAction->setEnabled(false);
    } else {
        QString path = trimPath(url.path());
        EPUBFile::PageInfo info = m_epub->getPathInfo(path);
        m_prevPageAction->setEnabled(info.hasPrev);
        m_nextPageAction->setEnabled(info.hasNext);
    }
}

int EPUBView::preferredWidth() const
{
    return m_preferredWidth;
}

int EPUBView::preferredHeight() const
{
    return m_preferredHeight;
}

void EPUBView::setPreferredWidth(int width)
{
    if (width != m_preferredWidth) {
        m_preferredWidth = width;
        resizeContent();
        emit preferredWidthChanged();
    }
}

void EPUBView::setPreferredHeight(int height)
{
    if (height != m_preferredHeight) {
        m_preferredHeight = height;
        resizeContent();
        emit preferredHeightChanged();
    }
}

void EPUBView::resizeContent()
{
    page()->setPreferredContentsSize(QSize(m_preferredWidth, m_preferredHeight));
}

qreal EPUBView::textSizeMultiplier() const
{
    return page()->mainFrame()->textSizeMultiplier();
}

void EPUBView::setTextSizeMultiplier(qreal factor)
{
    page()->mainFrame()->setTextSizeMultiplier(factor);
}

QString EPUBView::defaultFont() const
{
    return settings()->fontFamily(QWebSettings::StandardFont);
}

void EPUBView::setDefaultFont(const QString &font)
{
    settings()->setFontFamily(QWebSettings::StandardFont, font);
}

QString EPUBView::tocUrl()
{
    QByteArray doc;

    if (m_epub)
        doc = m_epub->tocDocument();
    return QLatin1String("data:application/x-dtbncx+xml;base64,") + QLatin1String(doc.toBase64());
}

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

#include "epubreadersettings.h"
#include <QDebug>

EPUBReaderSettings::EPUBReaderSettings(QObject *parent) :
    QSettings(parent)
{
}

qreal EPUBReaderSettings::textSizeMultiplier() const
{
    return value(QLatin1String("Reader/TextSizeMultiplier"), 1.5).toReal();
}

void EPUBReaderSettings::setTextSizeMultiplier(qreal ratio)
{
    if (ratio != textSizeMultiplier()) {
        setValue(QLatin1String("Reader/TextSizeMultiplier"), ratio);
        emit textSizeMultiplierChanged();
    }
}

QString EPUBReaderSettings::fontFamily() const
{
    return value(QLatin1String("Reader/FontFamily")).toString();
}

void EPUBReaderSettings::setFontFamily(const QString &family)
{
    if (family != fontFamily()) {
        setValue(QLatin1String("Reader/FontFamily"), family);
        emit fontFamilyChanged();
    }
}

int EPUBReaderSettings::colorIndex() const
{
    return value(QLatin1String("Reader/Background"), 0).toInt();
}

void EPUBReaderSettings::setColorIndex(int idx)
{
    if (idx != colorIndex()) {
        setValue(QLatin1String("Reader/Background"), idx);
        emit colorIndexChanged();
    }
}

typedef QPair<QString, QString> UrlDesc;
typedef QList<UrlDesc> UrlsList;

UrlsList EPUBReaderSettings::getLastURLs()
{
    UrlsList result;

    int size = beginReadArray(QLatin1String("Viewed Files"));
    for (int i = 0; i < size; i++) {
        setArrayIndex(i);
        QString fileName;
        QString url;

        fileName = value(QLatin1String("FileName")).toString();
        url = value(QLatin1String("Url")).toString();
        result.append(UrlDesc(fileName, url));
    }
    endArray();
    return result;
}

void EPUBReaderSettings::saveLastURL(const QString &fileName, const QString &url)
{
    qDebug() << "Save last url:" << fileName << url;
    if (fileName.isEmpty() || url.isEmpty())
        return;
    UrlsList urls = getLastURLs();

    int idx = -1;
    for (int i = 0; i < urls.length(); i++) {
        if (urls[i].first == fileName) {
            idx = i;
            break;
        }
    }

    if (idx != -1)
        urls.removeAt(idx);
    else if (urls.size() > 10)
        urls.removeLast();

    urls.prepend(UrlDesc(fileName, url));

    beginWriteArray(QLatin1String("Viewed Files"), urls.size());
    for (int i = 0; i < urls.size(); i++) {
        setArrayIndex(i);
        setValue(QLatin1String("FileName"), urls[i].first);
        setValue(QLatin1String("Url"), urls[i].second);
    }
    endArray();
}

QString EPUBReaderSettings::lastUrlForFile(const QString &path)
{
    UrlsList urls = getLastURLs();
    foreach (const UrlDesc &url, urls) {
        if (url.first == path)
            return url.second;
    }
    return QString();
}

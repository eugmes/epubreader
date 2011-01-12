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

#ifndef EPUBREADERSETTINGS_H
#define EPUBREADERSETTINGS_H

#include <QSettings>
#include <QUrl>

class EPUBReaderSettings : public QSettings {
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier NOTIFY textSizeMultiplierChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(int colorIndex READ colorIndex WRITE setColorIndex NOTIFY colorIndexChanged)
    Q_PROPERTY(Orientation windowOrientation READ windowOrientation WRITE setWindowOrientation NOTIFY windowOrientationChanged)
public:
    explicit EPUBReaderSettings(QObject *parent = 0);

    enum Orientation {AutoOrientation, LandscapeOrientation, PortraitOrientation};

    qreal textSizeMultiplier() const;
    void setTextSizeMultiplier(qreal ratio);

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    int colorIndex() const;
    void setColorIndex(int idx);

    Orientation windowOrientation() const;
    void setWindowOrientation(Orientation o);

    QUrl lastUrlForFile(const QString &path);
    QString lastOpenFile();

public Q_SLOTS:
    void saveLastURL(const QString &fileName, const QUrl &url);

private:
    QList<QPair<QString, QUrl> > getLastURLs();

Q_SIGNALS:
    void textSizeMultiplierChanged();
    void fontFamilyChanged();
    void colorIndexChanged();
    void windowOrientationChanged();
};

#endif

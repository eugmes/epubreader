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

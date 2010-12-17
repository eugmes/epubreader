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

#include "epubabstractxmlparser.h"

EPUBAbstractXMLParser::EPUBAbstractXMLParser() : QAbstractXmlReceiver()
{
}

void EPUBAbstractXMLParser::atomicValue(const QVariant &value)
{
    Q_UNUSED(value);
    /* empty */
}

void EPUBAbstractXMLParser::comment(const QString &value)
{
    Q_UNUSED(value);
    /* empty */
}

void EPUBAbstractXMLParser::endDocument()
{
    /* empty */
}

void EPUBAbstractXMLParser::endOfSequence()
{
    /* empty */
}

void EPUBAbstractXMLParser::namespaceBinding(const QXmlName &name)
{
    Q_UNUSED(name);
    /* empty */
}

void EPUBAbstractXMLParser::processingInstruction(const QXmlName &target, const QString &value)
{
    Q_UNUSED(target);
    Q_UNUSED(value);
    /* empty */
}

void EPUBAbstractXMLParser::startDocument()
{
    /* empty */
}

void EPUBAbstractXMLParser::startOfSequence()
{
    /* empty */
}

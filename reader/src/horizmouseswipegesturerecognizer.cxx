/* Copyright © 2011 Євгеній Мещеряков <eugen@debian.org>
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

#include "horizmouseswipegesturerecognizer.h"
#include "horizmouseswipegesture.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QWidget>

HorizMouseSwipeGestureRecognizer::HorizMouseSwipeGestureRecognizer() :
    QGestureRecognizer()
{
}

QGesture *HorizMouseSwipeGestureRecognizer::create(QObject *target)
{
    Q_UNUSED(target);
    return new HorizMouseSwipeGesture;
}

static const int horizMin = 200;
static const int vertMax = 50;

QGestureRecognizer::Result HorizMouseSwipeGestureRecognizer::recognize(QGesture *state, QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    HorizMouseSwipeGesture *g = static_cast<HorizMouseSwipeGesture *>(state);
    const QMouseEvent *ev = static_cast<QMouseEvent *>(event);
    const QGraphicsSceneMouseEvent *gsme = static_cast<QGraphicsSceneMouseEvent *>(event);

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        if (ev->buttons() == Qt::LeftButton) {
            g->m_offset = gsme->screenPos();
            g->setHotSpot(g->m_offset);
            return TriggerGesture;
        }
        return CancelGesture;

    case QEvent::MouseButtonPress:
        if (ev->buttons() == Qt::LeftButton) {
            g->m_offset = ev->globalPos();
            g->setHotSpot(g->m_offset);
            return TriggerGesture;
        }
        return CancelGesture;

    case QEvent::GraphicsSceneMouseMove:
        return Ignore;

    case QEvent::MouseMove:
        return Ignore;

    case QEvent::GraphicsSceneMouseRelease:
        if (g->state() != Qt::NoGesture) {
            QPoint newPos = gsme->screenPos();
            QPoint diff = newPos - g->m_offset;
            g->m_left = (diff.x() < 0);
            if ((abs(diff.x()) >= horizMin) && abs(diff.y()) <= vertMax)
                return FinishGesture;
        }
        return CancelGesture;

    case QEvent::MouseButtonRelease:
        if (g->state() != Qt::NoGesture) {
            QPoint newPos = ev->globalPos();
            QPoint diff = newPos - g->m_offset;
            g->m_left = (diff.x() < 0);
            if ((abs(diff.x()) >= horizMin) && abs(diff.y()) <= vertMax)
                return FinishGesture;
        }
        return CancelGesture;

    default:
        break;
    }

    return Ignore;
}

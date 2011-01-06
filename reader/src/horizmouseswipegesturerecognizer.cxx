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

static const int horizMin = 50;
static const int xyRatio = 2;
static const int gestureTimeout = 500;

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
            g->m_start = QTime::currentTime();
            g->setHotSpot(g->m_offset);
            return TriggerGesture;
        }
        return CancelGesture;

    case QEvent::MouseButtonPress:
        if (ev->buttons() == Qt::LeftButton) {
            g->m_offset = ev->globalPos();
            g->m_start = QTime::currentTime();
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
            if (g->m_start.msecsTo(QTime::currentTime()) > gestureTimeout)
                return CancelGesture;
            QPoint newPos = gsme->screenPos();
            QPoint diff = newPos - g->m_offset;
            int xDiff = diff.x();
            int yDiff = diff.y();
            g->m_left = (xDiff < 0);
            if ((abs(xDiff) >= horizMin) && (xyRatio * abs(yDiff) <= abs(xDiff)))
                return FinishGesture;
        }
        return CancelGesture;

    case QEvent::MouseButtonRelease:
        if (g->state() != Qt::NoGesture) {
            if (g->m_start.msecsTo(QTime::currentTime()) > gestureTimeout)
                return CancelGesture;
            QPoint newPos = ev->globalPos();
            QPoint diff = newPos - g->m_offset;
            int xDiff = diff.x();
            int yDiff = diff.y();
            g->m_left = (xDiff < 0);
            if ((abs(xDiff) >= horizMin) && (xyRatio * abs(yDiff) <= abs(xDiff)))
                return FinishGesture;
        }
        return CancelGesture;

    default:
        break;
    }

    return Ignore;
}

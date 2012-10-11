// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QLineF>
#include "circle.h"
#include <qmath.h>

Circle::Circle(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    const qreal x1 = (p2.x() + p1.x()) / 2;
    const qreal y11 = (p2.y() + p1.y()) / 2;
    const qreal dy1 = p2.x() - p1.x();
    const qreal dx1 = -(p2.y() - p1.y());
    const qreal x2 = (p3.x() + p2.x()) / 2;
    const qreal y2 = (p3.y() + p2.y()) / 2;
    const qreal dy2 = p3.x() - p2.x();
    const qreal dx2 = -(p3.y() - p2.y());
    const qreal ox = (y11 * dx1 * dx2 + x2 * dx1 * dy2 - x1 * dy1 * dx2 - y2 * dx1 * dx2) / (dx1 * dy2 - dy1 * dx2);
    const qreal oy = (ox - x1) * dy1 / dx1 + y11;
    mCenter = QPointF(ox - p1.x(), oy - p1.y());
    mRadius = qSqrt(mCenter.x() * mCenter.x() + mCenter.y() * mCenter.y());
}

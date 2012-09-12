// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QLineF>
#include "circle.h"


Circle::Circle(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    if (!isPerpendicular(p1, p2, p3))
        from3Points(p1, p2, p3);
    else if (!isPerpendicular(p1, p3, p2))
        from3Points(p1, p3, p2);
    else if (!isPerpendicular(p2, p1, p3))
        from3Points(p2, p1, p3);
    else if (!isPerpendicular(p2, p3, p1))
        from3Points(p2, p3, p1);
    else if (!isPerpendicular(p3, p2, p1))
        from3Points(p3, p2, p1);
    else if (!isPerpendicular(p3, p1, p2))
        from3Points(p3, p1, p2);
    else
        mRadius = -1;
}


qreal Circle::from3Points(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    const qreal yda = p2.y() - p1.y();
    const qreal xda = p2.x() - p1.x();
    const qreal ydb = p3.y() - p2.y();
    const qreal xdb = p3.x() - p2.x();
    if (qFuzzyIsNull(xda) && qFuzzyIsNull(ydb)) {
        mCenter = QPointF((p2.x() + p3.x()) / 2, (p1.y() + p2.y()) / 2);
        mRadius = QLineF(mCenter, p1).length();
        return mRadius;
    }
    const qreal aSlope = yda / xda;
    const qreal bSlope = ydb / xdb;
    if (qFuzzyIsNull(aSlope-bSlope))
        return -1;
    mCenter.setX((aSlope * bSlope * (p1.y() - p3.y()) + bSlope * (p1.x() + p2.x()) - aSlope*(p2.x() + p3.x())) / (2 * (bSlope - aSlope)));
    mCenter.setY(-(mCenter.x() - (p1.x() + p2.x()) / 2) / aSlope + (p1.y() + p2.y()) / 2);
    mRadius = QLineF(mCenter, p1).length();
    return mRadius;
}

bool Circle::isPerpendicular(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    const qreal yda = p2.y() - p1.y();
    const qreal xda = p2.x() - p1.x();
    const qreal ydb = p3.y() - p2.y();
    const qreal xdb = p3.x() - p2.x();
    if (qFuzzyIsNull(xda) && qFuzzyIsNull(ydb))
        return false;
    if (qFuzzyIsNull(yda))
        return true;
    if (qFuzzyIsNull(ydb))
        return true;
    if (qFuzzyIsNull(xda))
        return true;
    if (qFuzzyIsNull(xdb))
        return true;
    return false;
}

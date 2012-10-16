// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPolygonF>
#include <QLineF>
#include "circle.h"
#include "helper.h"


#define CHECK_AND_MAKE(a, b, c) (!isPerpendicular(poly.at(a), poly.at(b), poly.at(c))) \
                                from3Points(poly.at(a), poly.at(b), poly.at(c));


Circle::Circle(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    QPolygonF poly;
    poly << p1 << p2 << p3;
    qSort(poly.begin(), poly.end(), pointLessThan);
    if      CHECK_AND_MAKE(0, 1, 2)
    else if CHECK_AND_MAKE(0, 2, 1)
    else if CHECK_AND_MAKE(1, 0, 2)
    else if CHECK_AND_MAKE(1, 2, 0)
    else if CHECK_AND_MAKE(2, 0, 1)
    else if CHECK_AND_MAKE(2, 1, 0)
    else mRadius = -1;
}


void Circle::from3Points(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    const qreal yda = p2.y() - p1.y();
    const qreal xda = p2.x() - p1.x();
    const qreal ydb = p3.y() - p2.y();
    const qreal xdb = p3.x() - p2.x();
    if (qFuzzyIsNull(xda) && qFuzzyIsNull(ydb)) {
        mCenter = QPointF((p2.x() + p3.x()) / 2, (p1.y() + p2.y()) / 2);
        mRadius = QLineF(mCenter, p1).length();
        return;
    }
    const qreal aSlope = yda / xda;
    const qreal bSlope = ydb / xdb;
    if (!qFuzzyIsNull(aSlope - bSlope)) {
        mCenter.setX((aSlope * bSlope * (p1.y() - p3.y()) + bSlope * (p1.x() + p2.x()) - aSlope*(p2.x() + p3.x())) / (2 * (bSlope - aSlope)));
        mCenter.setY(-(mCenter.x() - (p1.x() + p2.x()) / 2) / aSlope + (p1.y() + p2.y()) / 2);
        mRadius = QLineF(mCenter, p1).length();
    }
    else {
        mRadius = -1;
    }
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

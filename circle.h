// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __CIRCLE_H_
#define __CIRCLE_H_

#include <QPointF>
#include <QLineF>

class Circle {
public:
    Circle(void)
        : mRadius(-1)
    { /* ... */ }
    Circle(const Circle& other)
        : mCenter(other.mCenter)
        , mRadius(other.mRadius)
    { /* ... */ }
    Circle(const QPointF& center, qreal radius)
        : mCenter(center)
        , mRadius(radius)
    { /* ... */ }
    Circle(const QPointF& p1, const QPointF& p2, const QPointF& p3);

    const QPointF& center(void) const { return mCenter; }
    qreal radius(void) const { return mRadius; }
    bool isValid(void) const { return mRadius >= 0; }
    void setCenter(const QPointF& center) { mCenter = center; }
    void setRadius(qreal radius) { mRadius = radius; }
    bool contains(const QPointF& p) const { return QLineF(mCenter, p).length() <= mRadius; }

private:
    QPointF mCenter;
    qreal mRadius;
};


#endif // __CIRCLE_H_

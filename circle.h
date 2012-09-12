// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __CIRCLE_H_
#define __CIRCLE_H_

#include <QPointF>

class Circle {
public:
    Circle(void) : mRadius(0.0) {}
    Circle(const Circle& other) : mCenter(other.mCenter), mRadius(other.mRadius) {}
    Circle(const QPointF& p1, const QPointF& p2, const QPointF& p3);

    const QPointF& center(void) const { return mCenter; }
    qreal radius(void) const { return mRadius; }

private: // methods
    qreal from3Points(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    bool isPerpendicular(const QPointF& p1, const QPointF& p2, const QPointF& p3);

private:
    QPointF mCenter;
    qreal mRadius;
};


#endif // __CIRCLE_H_

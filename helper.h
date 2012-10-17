// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __HELPER_H_
#define __HELPER_H_

#include <QString>
#include <QPointF>
#include <QPolygonF>

extern QString secondsToTime(int);
extern void avoidDuplicateFilename(QString& filename);
extern bool isConvexPolygon(const QPolygonF&);
extern QPolygonF convexHull(QPolygonF);

template <typename T>
inline T square(T x) { return x*x; }

enum StartDistribution {
    RandomDistribution = 0,
    TiledDistribution = 1,
    TiledWithColorHintDistribution = 2,
    ScatteredDistribution = 3,
    ScatteredWithColorHintDistribution = 4,
    TiledTrianglesWithColorHintDistribution = 5
};

inline bool pointLessThan(const QPointF& a, const QPointF& b)
{
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
}

inline qreal cross(const QPointF& O, const QPointF& A, const QPointF& B)
{
    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
}


#endif // __HELPER_H_

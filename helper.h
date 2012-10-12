// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __HELPER_H_
#define __HELPER_H_

#include <QString>
#include <QPolygonF>

extern QString secondsToTime(int);
extern void avoidDuplicateFilename(QString& filename);
extern bool isConvexPolygon(const QPolygonF&);

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

#endif // __HELPER_H_

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENOME_H_
#define __GENOME_H_

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QTextStream>
#include "random/mersenne_twister.h"

class Breeder;

class Genome
{
public:
    explicit Genome(bool randomize = false);

    Genome(const QPolygonF& polygon, const QColor& color)
        : mColor(color)
    {
        deepCopy(polygon);
    }

    Genome(const Genome& other)
        : mColor(other.mColor)
    {
        deepCopy(other.polygon());
    }

    inline const QColor& color(void) const { return mColor; }
    inline const QPolygonF& polygon(void) const { return mPolygon; }

    void mutate(void);

    QPolygonF convexHull(void) const;
    QVector<Genome> bisect(void) const;
    QVector<Genome> triangulize(void) const;

private:
    QPolygonF mPolygon;
    QColor mColor;

    bool willMutate(int rate) const;

    void deepCopy(const QPolygonF& polygon);

};


QTextStream& operator<<(QTextStream&, const Genome&);


#endif // __GENOME_H_

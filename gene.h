// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENE_H_
#define __GENE_H_

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QTextStream>
#include "random/mersenne_twister.h"


class Gene
{
public:
    explicit Gene(bool randomize = false);

    Gene(const QPolygonF& polygon, const QColor& color)
        : mColor(color)
    {
        deepCopy(polygon);
    }

    Gene(const Gene& other)
        : mColor(other.mColor)
    {
        deepCopy(other.polygon());
    }

    inline const QColor& color(void) const { return mColor; }
    inline const QPolygonF& polygon(void) const { return mPolygon; }

    void mutate(void);

    QPolygonF convexHull(void) const;
    QVector<Gene> bisect(void) const;
    QVector<Gene> triangulize(void) const;

private:
    QPolygonF mPolygon;
    QColor mColor;

    bool willMutate(int rate) const;

    void deepCopy(const QPolygonF& polygon);

};


QTextStream& operator<<(QTextStream&, const Gene&);


#endif // __GENE_H_

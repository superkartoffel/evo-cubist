// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENOME_H_
#define __GENOME_H_

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QDataStream>

class Breeder;

class Genome
{
public:
    Genome(void);
    Genome(const Breeder*);
    Genome(const Genome&);

    const QColor& color(void) const { return mColor; }
    const QPolygonF& polygon(void) const { return mPolygon; }

    static const int NUM_POINTS = 3;

    void mutate(void);

    QDataStream& operator<<(QDataStream&);

private:
    const Breeder* mBreeder;
    QPolygonF mPolygon;
    QColor mColor;

    static qreal rnd(void);
    bool willMutate(void);
};


#endif // __GENOME_H_

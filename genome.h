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
    Genome(const Genome& other);
    Genome(QPolygonF polygon, QColor color);

    inline const QColor& color(void) const { return mColor; }
    inline const QPolygonF& polygon(void) const { return mPolygon; }

    void mutate(void);

private:
    QPolygonF mPolygon;
    QColor mColor;

    bool willMutate(int rate) const;
};


QTextStream& operator<<(QTextStream&, const Genome&);


#endif // __GENOME_H_

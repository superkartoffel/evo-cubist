// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENOME_H_
#define __GENOME_H_

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QPolygonF>
#include <QTextStream>

class Breeder;

class Genome
{
public:
    inline Genome(void)
        : mBreeder(NULL) { /* ... */ }
    inline Genome(const Genome& o)
        : mBreeder(o.mBreeder)
        , mPolygon(o.mPolygon)
        , mColor(o.mColor) { /* ... */ }
    Genome(Breeder*);

    inline const QColor& color(void) const { return mColor; }
    inline const QPolygonF& polygon(void) const { return mPolygon; }
    inline unsigned int size(void) const { return polygon().size(); }

    void setColor(const QColor&);
    void setPolygon(const QPolygonF&);

    static const int MIN_NUM_POINTS = 3;
    static const int MAX_NUM_POINTS = 9;

    void mutate(void);

private:
    Breeder* mBreeder;
    QPolygonF mPolygon;
    QColor mColor;

    bool willMutate(void);
};


QTextStream& operator<<(QTextStream&, const Genome&);
QTextStream& operator>>(QTextStream&, const Genome&);



#endif // __GENOME_H_

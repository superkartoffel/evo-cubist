// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "genome.h"
#include "breeder.h"
#include <QtGlobal>


const qreal Genome::POINT_MUTATION_SCALE = 0.02;
const int Genome::RGB_MUTATION_SCALE = 12;


Genome::Genome(void)
{
    // ...
}


Genome::Genome(const Breeder* breeder)
    : mBreeder(breeder)
{
    for (int x = 0; x < NUM_POINTS; ++x)
        mPolygon.append(QPointF(rnd(), rnd()));
    mColor.setRgb(qrand() % 255, qrand() % 255, qrand() % 255, 10 + qrand() % 50);
}


Genome::Genome(const Genome& o)
    : mBreeder(o.mBreeder)
    , mPolygon(o.mPolygon)
    , mColor(o.mColor)
{
    // ...
}


qreal Genome::rnd(void) {
    return (qreal)qrand() / (qreal)RAND_MAX;
}


void Genome::mutate(void)
{
    for (QPolygonF::iterator p = mPolygon.begin(); p != mPolygon.end(); ++p) {
        qreal x2 = p->x() + mBreeder->mdXY * (rnd() - 0.5);
        qreal y2 = p->y() + mBreeder->mdXY * (rnd() - 0.5);
        p->setX((x2 < 0)? 0.0 : ((x2 > 1)? 1.0 : x2));
        p->setY((y2 < 0)? 0.0 : ((y2 > 1)? 1.0 : y2));
    }
    const int r = 0xff & (qrand() % mBreeder->mdR + mColor.red());
    const int g = 0xff & (qrand() % mBreeder->mdG + mColor.green());
    const int b = 0xff & (qrand() % mBreeder->mdB + mColor.blue());
    const int a = qrand() % RGB_MUTATION_SCALE + mColor.alpha();
    mColor.setRgb(r, g, b, (a < 10)? 10 : ((a > 60)? 60 : a));
}

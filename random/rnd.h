// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RND_H_
#define __RND_H_

#include <QtCore>
#include "mersenne_twister.h"


extern MT::MersenneTwister rng;


namespace RAND {

extern void initialize(void);


inline unsigned int rnd(void)
{
    return rng.next();
}


inline unsigned int rnd(int a)
{
    Q_ASSERT(a != 0);
    return rng.next() % a;
}


inline int rnd(int a, int b)
{
    Q_ASSERT(b >= a);
    return a + RAND::rnd(1 + b - a);
}


inline double rnd1(void)
{
    return (double)RAND::rnd() / rng.max();
}


inline double rnd1(qreal a, qreal b)
{
    Q_ASSERT(b >= a);
    return a + RAND::rnd1() * (b - a);
}


inline int dInt(int v, int deltaMax)
{
    const int r = RAND::rnd(-int(deltaMax), int(deltaMax));
    return v + r;
}


inline int dInt(int v, int deltaMax, int L /* lower boundary */, int U /* upper boundary */)
{
    return qBound(L, dInt(v, deltaMax), U);
}


inline qreal dReal(qreal v, qreal deltaMax)
{
    return v + RAND::rnd1(-deltaMax, deltaMax);
}


inline qreal dReal(qreal v, qreal deltaMax, qreal L /* lower boundary */, qreal U /* upper boundary */)
{
    return qBound(L, dReal(v, deltaMax), U);
}

}

#endif // __RND_H_

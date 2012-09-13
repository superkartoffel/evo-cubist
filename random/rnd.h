// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RND_H_
#define __RND_H_

#include <QtCore>
#include "mersenne_twister.h"

extern MT::MersenneTwister rng;

inline unsigned int random(void)
{
    return rng.next();
}

inline unsigned int random(int a)
{
    Q_ASSERT(a != 0);
    return rng.next() % a;
}

inline int random(int a, int b)
{
    Q_ASSERT(b > a);
    return a + random(1 + b - a);
}

inline double random1(void)
{
    return (double)random() / rng.max();
}

inline double random1(qreal a, qreal b)
{
    Q_ASSERT(b > a);
    return a + random1() * (b - a);
}

inline int dInt(int v, int deltaMax)
{
    const int r = random(-(int)deltaMax, (int)deltaMax);
    return v + r;
}

inline int dInt(int v, int deltaMax, int L /* lower boundary */, int U /* upper boundary */)
{
    const int r = dInt(v, deltaMax);
    return (r < L)? L : ((r > U)? U : r);
}

inline qreal dReal(qreal v, qreal deltaMax)
{
    return v + random1(-deltaMax, deltaMax);
}

inline qreal dReal(qreal v, qreal deltaMax, qreal L /* lower boundary */, qreal U /* upper boundary */)
{
    const qreal r = dReal(v, deltaMax);
    return (r < L)? L : ((r > U)? U : r);
}

#endif // __RND_H_

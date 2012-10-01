// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RND_H_
#define __RND_H_

#include <QtCore>
#include "mersenne_twister.h"


extern MT::MersenneTwister rng;


template <typename T>
inline T clamp(T v, T L, T U)
{
    return (v < L)? L : ((v > U)? U : v);
}


inline unsigned int randomu(void)
{
    return rng.next();
}


inline unsigned int randomu(int a)
{
    Q_ASSERT(a != 0);
    return rng.next() % a;
}


inline int randomu(int a, int b)
{
    Q_ASSERT(b >= a);
    return a + randomu(1 + b - a);
}


inline double random1(void)
{
    return (double)randomu() / rng.max();
}


inline double random1(qreal a, qreal b)
{
    Q_ASSERT(b >= a);
    return a + random1() * (b - a);
}


inline int dInt(int v, int deltaMax)
{
    const int r = randomu(-int(deltaMax), int(deltaMax));
    return v + r;
}


inline int dInt(int v, int deltaMax, int L /* lower boundary */, int U /* upper boundary */)
{
    return clamp(dInt(v, deltaMax), L, U);
}


inline qreal dReal(qreal v, qreal deltaMax)
{
    return v + random1(-deltaMax, deltaMax);
}


inline qreal dReal(qreal v, qreal deltaMax, qreal L /* lower boundary */, qreal U /* upper boundary */)
{
    return clamp(dReal(v, deltaMax), L, U);
}

#endif // __RND_H_

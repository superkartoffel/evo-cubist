// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MERSENNETWISTER_H_
#define __MERSENNETWISTER_H_

#include <QtCore>
#include "abstract_random_number_generator.h"

namespace MT {

    class MersenneTwister : public randomtools::UIntRandomNumberGenerator
    {
    public:
        MersenneTwister(void) {}
        unsigned int operator()();
        inline unsigned int next(void) { return (*this)(); }
        void seed(unsigned int _Seed = 9U);

    private:
        static const int N = 624;
        static const int M = 397;
        static const unsigned int LO = 0x7fffffffU;
        static const unsigned int HI = 0x80000000U;
        static const unsigned int A[2];
        unsigned int y[N];
        int index;

    private: // methods
        void warmup();
    };


    extern MersenneTwister rng;

    inline unsigned int random(void)
    {
        return rng.next();
    }
    inline unsigned int random(unsigned int a)
    {
        Q_ASSERT(a != 0);
        return rng.next() % a;
    }
    inline unsigned int random(int a, int b)
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
    inline unsigned int deviatei(unsigned int v, unsigned int deltaMax)
    {
        return v + MT::random(-(int)deltaMax, (int)deltaMax);
    }
    inline unsigned int deviatei(unsigned int v, unsigned int deltaMax, unsigned int L /* lower boundary */, unsigned int U /* upper boundary */)
    {
        const unsigned int r = deviatei(v, deltaMax);
        return (r < L)? L : ((r > U)? U : r);
    }
    inline qreal deviater(qreal v, qreal deltaMax)
    {
        return v + MT::random1(-deltaMax, deltaMax);
    }
    inline qreal deviater(qreal v, qreal deltaMax, qreal L /* lower boundary */, qreal U /* upper boundary */)
    {
        const qreal r = deviater(v, deltaMax);
        return (r < L)? L : ((r > U)? U : r);
    }

}

#endif //  __MERSENNETWISTER_H_

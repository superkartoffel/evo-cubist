// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MERSENNETWISTER_H_
#define __MERSENNETWISTER_H_

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


}

#endif //  __MERSENNETWISTER_H_

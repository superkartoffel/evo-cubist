// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.


#ifndef __ABSTRACTNUMBERGENERATOR_H_
#define __ABSTRACTNUMBERGENERATOR_H_

#ifdef _WIN32
#define _CRT_RAND_S
#include "compat-win32.h"
#else
#include <cstdio>
#include <ctime>
#endif

#include <cstdlib>
#include <limits>

namespace randomtools {

    template <typename VariateType>
    class RandomNumberGenerator
    {
    public:
        RandomNumberGenerator(void) {}
        virtual ~RandomNumberGenerator() {}
        virtual VariateType operator()(void) = 0;
        virtual void seed(VariateType) { }
        virtual inline VariateType min(void) const { return std::numeric_limits<VariateType>::min(); }
        virtual inline VariateType max(void) const { return std::numeric_limits<VariateType>::max(); }

        static VariateType makeSeed(void);
    };

    typedef RandomNumberGenerator<unsigned long long> UInt64RandomNumberGenerator;
    typedef RandomNumberGenerator<unsigned int> UIntRandomNumberGenerator;

}

#endif //  __ABSTRACTNUMBERGENERATOR_H_

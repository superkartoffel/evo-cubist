// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.


#ifndef __ABSTRACTNUMBERGENERATOR_H_
#define __ABSTRACTNUMBERGENERATOR_H_

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

    typedef RandomNumberGenerator<unsigned int> UIntRandomNumberGenerator;

}

#endif //  __ABSTRACTNUMBERGENERATOR_H_

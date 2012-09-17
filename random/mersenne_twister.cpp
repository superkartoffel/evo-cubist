// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "mersenne_twister.h"

namespace MT {


    void MersenneTwister::seed(unsigned int _Seed)
    {
        unsigned int r = _Seed;
        unsigned int s = 3402U;
        for (int i = 0; i < N; ++i) {
            r = 509845221U * r + 3U;
            s *= s + 1U;
            y[i] = s + (r >> 10);
        }
        index = 0;
        warmup();
    }


    void MersenneTwister::warmup(void)
    {
        for (int i = 0; i < 10000; ++i)
            (*this)();
    }


    unsigned int MersenneTwister::operator()()
    {
        if (index >= N) {
            unsigned int h;
            for (int k = 0 ; k < N-M ; ++k) {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+M] ^ (h >> 1) ^ A[h & 1];
            }
            for (int k = N-M ; k < N-1 ; ++k) {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+(M-N)] ^ (h >> 1) ^ A[h & 1];
            }
            h = (y[N-1] & HI) | (y[0] & LO);
            y[N-1] = y[M-1] ^ (h >> 1) ^ A[h & 1];
            index = 0;
        }

        unsigned int e = y[index++];
        e ^= (e >> 11);
        e ^= (e << 7) & 0x9d2c5680;
        e ^= (e << 15) & 0xefc60000;
        e ^= (e >> 18);
        return e;
    }


    const unsigned int MersenneTwister::A[2] = { 0, 0x9908b0df };
}

// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rnd.h"

#include <QDateTime>

MT::MersenneTwister rng;

namespace RAND {

void initialize(void)
{
    rng.seed(QDateTime::currentDateTime().toTime_t());
}

}

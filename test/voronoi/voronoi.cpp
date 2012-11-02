// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "voronoi.h"

Voronoi& operator<<(Voronoi& v, const Point& p)
{
    v.add(p);
    return v;
}


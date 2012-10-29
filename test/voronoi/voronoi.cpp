#include "voronoi.h"

Voronoi& operator<<(Voronoi& v, const Point& p)
{
    v.add(p);
    return v;
}


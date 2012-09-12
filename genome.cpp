// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "random/mersenne_twister.h"
#include "genome.h"
#include "breedersettings.h"
#include "random/rnd.h"
#include <QtGlobal>
#include <QtCore/QDebug>


Genome::Genome(bool randomize)
{
    if (randomize) {
        const int N = random(gBreederSettings.minPointsPerGenome(), gBreederSettings.maxPointsPerGenome());
        for (int x = 0; x < N; ++x)
            mPolygon.append(QPointF(random1(), random1()));
        mColor.setRgb(random(256), random(256), random(256), random(gBreederSettings.minA(), gBreederSettings.maxA()));
    }
}


void Genome::deepCopy(const QPolygonF& polygon)
{
    mPolygon.reserve(polygon.size());
    for (QPolygonF::const_iterator p = polygon.constBegin(); p != polygon.constEnd(); ++p)
        mPolygon.append(*p);
}


inline bool Genome::willMutate(int rate) const {
    return (random(rate)) == 0;
}


void Genome::mutate(void)
{
    if (willMutate(gBreederSettings.pointEmergenceProbability()) && mPolygon.size() < gBreederSettings.maxPointsPerGenome())
        mPolygon.append(QPointF(random1(), random1()));
    if (willMutate(gBreederSettings.pointKillProbability()) && mPolygon.size() > gBreederSettings.minPointsPerGenome())
        mPolygon.remove(random(mPolygon.size()));
    for (QPolygonF::iterator p = mPolygon.begin(); p != mPolygon.end(); ++p) {
        if (willMutate(gBreederSettings.pointMutationProbability())) {
            p->setX(dReal(p->x(), gBreederSettings.dXY(), 0.0, 1.0));
            p->setY(dReal(p->y(), gBreederSettings.dXY(), 0.0, 1.0));
        }
    }
    if (willMutate(gBreederSettings.colorMutationProbability())) {
        const int r = dInt(mColor.red(), gBreederSettings.dR(), 0, 255);
        const int g = dInt(mColor.green(), gBreederSettings.dR(), 0, 255);
        const int b = dInt(mColor.blue(), gBreederSettings.dR(), 0, 255);
        const int a = dInt(mColor.alpha(), gBreederSettings.dA(), gBreederSettings.minA(), gBreederSettings.maxA());
        mColor.setRgb(r, g, b, a);
    }
}


QTextStream& operator<< (QTextStream& s, const Genome& genome)
{
    const QColor& color = genome.color();
    s << "{\n"
      << "  \"color\": { " << QString("\"r\": %1, \"g\": %2, \"b\": %3, \"a\": %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alphaF()) << " },\n";
    s << "  \"vertices\": [\n";
    for (QPolygonF::const_iterator p = genome.polygon().constBegin(); p != genome.polygon().constEnd(); ++p) {
        s << "    { " << QString("\"x\": %1, \"y\": %2").arg(p->x()).arg(p->y()) << " }";
        if ((p+1) != genome.polygon().constEnd())
            s << ",";
        s << "\n";
    }
    s << "  ]\n"
      << "}";
    return s;
}

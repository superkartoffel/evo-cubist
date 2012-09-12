// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "random/mersenne_twister.h"
#include "genome.h"
#include "breedersettings.h"
#include <QtGlobal>
#include <QtCore/QDebug>


Genome::Genome(bool randomize)
{
    if (randomize) {
        const int N = gBreederSettings.minPointsPerGenome() + MT::random() % (1 + gBreederSettings.maxPointsPerGenome() - gBreederSettings.minPointsPerGenome());
        for (int x = 0; x < N; ++x)
            mPolygon.append(QPointF(MT::random1(), MT::random1()));
        mColor.setRgb(MT::random() % 255, MT::random() % 255, MT::random() % 255, gBreederSettings.minA() + MT::random() % (gBreederSettings.maxA() - gBreederSettings.minA()));
    }
}


void Genome::deepCopy(const QPolygonF& polygon)
{
    mPolygon.reserve(polygon.size());
    for (QPolygonF::const_iterator p = polygon.constBegin(); p != polygon.constEnd(); ++p)
        mPolygon.append(*p);
}


inline bool Genome::willMutate(int rate) const {
    return (MT::random() % rate) == 0;
}


void Genome::mutate(void)
{
    if (willMutate(gBreederSettings.pointEmergenceProbability()) && mPolygon.size() < gBreederSettings.maxPointsPerGenome())
        mPolygon.append(QPointF(MT::random1(), MT::random1()));
    if (willMutate(gBreederSettings.pointKillProbability()) && mPolygon.size() > gBreederSettings.minPointsPerGenome())
        mPolygon.remove(MT::random() % mPolygon.size());
    for (QPolygonF::iterator p = mPolygon.begin(); p != mPolygon.end(); ++p) {
        if (willMutate(gBreederSettings.pointMutationProbability())) {
            p->setX(MT::deviater(p->x(), gBreederSettings.dXY(), 0.0, 1.0));
            p->setY(MT::deviater(p->y(), gBreederSettings.dXY(), 0.0, 1.0));
        }
    }
    if (willMutate(gBreederSettings.colorMutationProbability())) {
        const int r = MT::deviatei(mColor.red(), gBreederSettings.dR(), 0, 255);
        const int g = MT::deviatei(mColor.green(), gBreederSettings.dR(), 0, 255);
        const int b = MT::deviatei(mColor.blue(), gBreederSettings.dR(), 0, 255);
        const int a = MT::deviatei(mColor.alpha(), gBreederSettings.dA(), gBreederSettings.minA(), gBreederSettings.maxA());
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

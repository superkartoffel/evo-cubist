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
        const int N = gBreederSettings.minPointsPerGenome() + MT::random() % (gBreederSettings.maxPointsPerGenome() - gBreederSettings.minPointsPerGenome());
        for (int x = 0; x < N; ++x)
            mPolygon.append(QPointF(MT::random1(), MT::random1()));
        mColor.setRgb(MT::random() % 255, MT::random() % 255, MT::random() % 255, gBreederSettings.minA() + MT::random() % (gBreederSettings.maxA() - gBreederSettings.minA()));
    }
}


//Genome Genome::operator=(Genome& other) {
//    QPolygonF polygon;
//    polygon.reserve(other.polygon().size());
//    qDebug() << "Genome::clone() " << mPolygon.size() << "elements";
//    for (QPolygonF::const_iterator p = mPolygon.constBegin(); p != mPolygon.constEnd(); ++p)
//        polygon.append(*p);
//    return Genome(polygon, mColor);
//}


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
            qreal x2 = p->x() + gBreederSettings.dXY() * (MT::random1() - 0.5);
            qreal y2 = p->y() + gBreederSettings.dXY() * (MT::random1() - 0.5);
            p->setX((x2 < 0.0)? 0.0 : ((x2 > 1.0)? 1.0 : x2));
            p->setY((y2 < 0.0)? 0.0 : ((y2 > 1.0)? 1.0 : y2));
        }
    }
    if (willMutate(gBreederSettings.colorMutationProbability())) {
        const int r = 0xff & (MT::random() % gBreederSettings.dR() + mColor.red());
        const int g = 0xff & (MT::random() % gBreederSettings.dG() + mColor.green());
        const int b = 0xff & (MT::random() % gBreederSettings.dB() + mColor.blue());
        const int a = gBreederSettings.minA() + (MT::random() % gBreederSettings.dA() + mColor.alpha()) % (gBreederSettings.maxA() - gBreederSettings.minA());
        mColor.setRgb(r, g, b, a);
    }
}


void Genome::setColor(const QColor& color)
{
    mColor = color;
}


void Genome::setPolygon(const QPolygonF& polygon)
{
    mPolygon = polygon;
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

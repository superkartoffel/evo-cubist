// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QtCore/QDebug>
#include <limits>
#include "gene.h"
#include "breedersettings.h"
#include "random/rnd.h"
#include "circle.h"


Gene::Gene(bool randomize)
{
    if (randomize) {
        const int N = RAND::rnd(gSettings.minPointsPerGene(), gSettings.maxPointsPerGene());
        for (int x = 0; x < N; ++x)
            mPolygon.append(QPointF(RAND::rnd1(), RAND::rnd1()));
        mColor.setRgb(RAND::rnd(256), RAND::rnd(256), RAND::rnd(256), RAND::rnd(gSettings.minA(), gSettings.maxA()));
    }
}


void Gene::deepCopy(const QPolygonF& polygon)
{
    mPolygon.reserve(polygon.size());
    for (QPolygonF::const_iterator p = polygon.constBegin(); p != polygon.constEnd(); ++p)
        mPolygon.append(*p);
}


QVector<Gene> Gene::splice(void) const
{
    Q_ASSERT(mPolygon.size() >= 3);
    return (mPolygon.size() == 3)
            ? bisect()
            : triangulize();
}


/// cut triangle in halves by slicing it from the middle of the longest leg to the opposite edge
QVector<Gene> Gene::bisect(void) const
{
    Q_ASSERT(mPolygon.size() == 3);
    QPolygonF triangle = mPolygon;
    triangle.append(mPolygon.at(0));
    triangle.append(mPolygon.at(1));
    // find longest leg
    QPolygonF::const_iterator p0 = triangle.constBegin();
    qreal possibleLongestLeg = QLineF(*p0, *(p0+1)).length();
    for (QPolygonF::const_iterator p = triangle.constBegin()+1; p != triangle.constEnd()-1; ++p) {
        const qreal l = QLineF(*p, *(p+1)).length();
        if (l > possibleLongestLeg) {
            possibleLongestLeg = l;
            p0 = p;
        }
    }
    QPointF mid = (*p0 + *(p0+1)) / 2;
    QPolygonF triangle1;
    triangle1 << *p0 << mid << *(p0+2);
    QPolygonF triangle2;
    triangle2 << *(p0+1) << *(p0+2) << mid;
    QVector<Gene> result;
    result << Gene(triangle1, mColor) << Gene(triangle2, mColor);
    return result;
}


QPolygonF Gene::evaluateTriangle(int i, int j, int k) const
{
    Circle circle(mPolygon.at(i), mPolygon.at(j), mPolygon.at(k));
    for (int w = 0; w < mPolygon.size(); ++w) {
        if (w == i || w == j || w == k)
            continue;
        if (circle.contains(mPolygon.at(w)))
            return QPolygonF();
    }
    QPolygonF result;
    result << mPolygon.at(i) << mPolygon.at(j) << mPolygon.at(k);
    return result;
}


/// convert point cloud to triangles (dumb O(n^4) implementation)
/// see http://de.wikipedia.org/wiki/Delaunay-Triangulation
QVector<Gene> Gene::triangulize(void) const
{
    Q_ASSERT(mPolygon.size() > 3);
    QVector<Gene> result;
    for (int i = 0; i < mPolygon.size()-2; ++i) {
        for (int j = i + 1; j < mPolygon.size()-1; ++j) {
            for (int k = j + 1; k < mPolygon.size(); ++k) {
                const QPolygonF& triangle = evaluateTriangle(i, j, k);
                if (!triangle.isEmpty())
                    result << Gene(triangle, mColor);
            }
        }
    }
    return result;
}


inline bool Gene::willMutate(int probability) const {
    return RAND::rnd(probability) == 0;
}


void Gene::randomlyTranslatePoint(QPointF& p)
{
    p.setX(RAND::dReal(p.x(), gSettings.dXY(), 0.0, 1.0));
    p.setY(RAND::dReal(p.y(), gSettings.dXY(), 0.0, 1.0));
}


void Gene::mutate(void)
{
    // emerge
    if (willMutate(gSettings.pointEmergenceProbability()) && mPolygon.size() < gSettings.maxPointsPerGene()) {
        const int i = RAND::rnd(mPolygon.size());
        const int j = (i+1) % mPolygon.size();
        const QPointF& p0 = mPolygon.at(i);
        const QPointF& p1 = mPolygon.at(j);
        QPointF newP = (p0 + p1) / 2;
        randomlyTranslatePoint(newP);
        mPolygon.insert(j, newP);
    }
    // kill
    if (willMutate(gSettings.pointKillProbability()) && mPolygon.size() > gSettings.minPointsPerGene())
        mPolygon.remove(RAND::rnd(mPolygon.size()));
    // translate
    for (QPolygonF::iterator p = mPolygon.begin(); p != mPolygon.end(); ++p) {
        if (willMutate(gSettings.pointMutationProbability()))
            randomlyTranslatePoint(*p);
    }
    // change color
    if (willMutate(gSettings.colorMutationProbability())) {
        const int r = RAND::dInt(mColor.red(), gSettings.dR(), 0, 255);
        const int g = RAND::dInt(mColor.green(), gSettings.dG(), 0, 255);
        const int b = RAND::dInt(mColor.blue(), gSettings.dB(), 0, 255);
        const int a = RAND::dInt(mColor.alpha(), gSettings.dA(), gSettings.minA(), gSettings.maxA());
        mColor.setRgb(r, g, b, a);
    }
}


QTextStream& operator<< (QTextStream& s, const Gene& gene)
{
    const QColor& color = gene.color();
    s << "{\n"
      << "  \"color\": { " << QString("\"r\": %1, \"g\": %2, \"b\": %3, \"a\": %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alphaF()) << " },\n";
    s << "  \"vertices\": [\n";
    for (QPolygonF::const_iterator p = gene.polygon().constBegin(); p != gene.polygon().constEnd(); ++p) {
        s << "    { " << QString("\"x\": %1, \"y\": %2").arg(p->x()).arg(p->y()) << " }";
        if ((p+1) != gene.polygon().constEnd())
            s << ",";
        s << "\n";
    }
    s << "  ]\n"
      << "}";
    return s;
}

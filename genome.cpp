// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QtCore/QDebug>
#include <limits>
#include "genome.h"
#include "breedersettings.h"
#include "random/rnd.h"


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


/// cut triangle in halves
QVector<Genome> Genome::bisect(void) const
{
    Q_ASSERT(mPolygon.size() == 3);
    QVector<Genome> result;
    QPolygonF triangle = mPolygon;
    triangle.append(mPolygon.first());
    triangle.prepend(mPolygon.last());
    // find longest leg
    QPolygonF::const_iterator p0 = NULL;
    qreal longestLength = std::numeric_limits<qreal>::max();
    for (QPolygonF::const_iterator p = mPolygon.constBegin()+1; p != mPolygon.constEnd()-1; ++p) {
        qreal l = QLineF(*p, *(p+1)).length();
        if (l < longestLength) {
            longestLength = l;
            p0 = p;
        }
    }
    QPointF mid = (*p0 + *(p0+1)) / 2;
    QPolygonF triangle1;
    triangle1 << mid << *(p0-1) << *p0;
    QPolygonF triangle2;
    triangle2 << mid << *(p0-1) << *(p0+1);
    result << Genome(triangle1, mColor) << Genome(triangle2, mColor);
    return result;
}


/// convert point cloud to triangles
/// see http://de.wikipedia.org/wiki/Delaunay-Triangulation
QVector<Genome> Genome::triangulize(void) const
{
    QVector<Genome> result;
    QPolygonF Q = mPolygon;
    // TODO ...
    return result;
}


inline int cross(const QPointF& O, const QPointF& A, const QPointF& B)
{
    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
}


inline bool pointLessThan(const QPointF& a, const QPointF& b)
{
    return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
}


QPolygonF Genome::convexHull(void) const
{
    QPolygonF P = mPolygon;
    int n = P.size();
    int k = 0;
    QPolygonF H(2*n);

    // sort points lexicographically
    qSort(P.begin(), P.end(), pointLessThan);

    // build lower hull
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0)
            k--;
        H[k++] = P[i];
    }
    // build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0)
            k--;
        H[k++] = P[i];
    }

    H.resize(k-1);
    return H;
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

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "genome.h"
#include "breeder.h"
#include <QtGlobal>
#include <QtCore/QDebug>


Genome::Genome(Breeder* breeder)
    : mBreeder(breeder)
{
    Q_ASSERT(mBreeder != NULL);
    const int N = MIN_NUM_POINTS + mBreeder->random() % (MAX_NUM_POINTS - MIN_NUM_POINTS);
    for (int x = 0; x < N; ++x)
        mPolygon.append(QPointF(mBreeder->random1(), mBreeder->random1()));
    mColor.setRgb(mBreeder->random() % 255, mBreeder->random() % 255, mBreeder->random() % 255, 10 + mBreeder->random() % 50);
}


inline bool Genome::willMutate(void) {
    return (mBreeder->random() % mBreeder->mMutationRate) == 0;
}


void Genome::mutate(void)
{
    if (willMutate() && mPolygon.size() < 9) {
        mPolygon.append(QPointF(mBreeder->random1(), mBreeder->random1()));
    }
    if (willMutate() && mPolygon.size() > 3) {
        mPolygon.remove(mBreeder->random() % mPolygon.size());
    }
    for (QPolygonF::iterator p = mPolygon.begin(); p != mPolygon.end(); ++p) {
        if (willMutate()) {
            qreal x2 = p->x() + mBreeder->mdXY * (mBreeder->random1() - 0.5);
            qreal y2 = p->y() + mBreeder->mdXY * (mBreeder->random1() - 0.5);
            p->setX((x2 < 0.0)? 0.0 : ((x2 > 1.0)? 1.0 : x2));
            p->setY((y2 < 0.0)? 0.0 : ((y2 > 1.0)? 1.0 : y2));
        }
    }
    if (willMutate()) {
        const int r = 0xff & (mBreeder->random() % mBreeder->mdR + mColor.red());
        const int g = 0xff & (mBreeder->random() % mBreeder->mdG + mColor.green());
        const int b = 0xff & (mBreeder->random() % mBreeder->mdB + mColor.blue());
        const int a = mBreeder->random() % mBreeder->mdA + mColor.alpha();
        mColor.setRgb(r, g, b, (a < 10)? 10 : ((a > 60)? 60 : a));
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

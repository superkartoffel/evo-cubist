// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <QImage>
#include <QPainter>
#include "dna.h"


inline unsigned int rgbDelta(QRgb c1, QRgb c2)
{
    const int r = qRed(c1) - qRed(c2);
    const int g = qGreen(c1) - qGreen(c2);
    const int b = qBlue(c1) - qBlue(c2);
    return r*r + g*g + b*b;
}


class Individual {
public:
    Individual(void)
        : mOriginal(NULL)
        , mFitness(ULONG_MAX)
    { /* ... */ }

    Individual(DNA dna, const QImage& original)
        : mDNA(dna)
        , mOriginal(&original)
        , mFitness(ULONG_MAX)
        , mGenerated(original.size(), original.format())
    { /* ... */ }

    inline QImage& generated(void) { return mGenerated; }
    inline const DNA& dna(void) const { return mDNA; }
    inline unsigned long fitness(void) const { return mFitness; }
    inline void mutate(void) {
        mDNA.mutate();
        QPainter p(&mGenerated);
        p.setPen(Qt::transparent);
        p.setBrush(Qt::white);
        p.drawRect(0, 0, mGenerated.width(), mGenerated.height());
        p.setRenderHint(QPainter::Antialiasing);
        p.scale(mGenerated.width(), mGenerated.height());
        for (DNAType::const_iterator genome = mDNA.constBegin(); genome != mDNA.constEnd(); ++genome) {
            p.setBrush(genome->color());
            p.drawPolygon(genome->polygon());
        }
        mFitness = 0;
        const int N = mOriginal->height() * mOriginal->width();
        const QRgb* o = reinterpret_cast<const QRgb*>(mOriginal->bits());
        const QRgb* const oEnd  = o + N;
        const QRgb* g = reinterpret_cast<const QRgb*>(mGenerated.bits());
        while (o < oEnd)
            mFitness += rgbDelta(*o++, *g++);
    }

private:
    DNA mDNA;
    const QImage* mOriginal;
    unsigned long mFitness;
    QImage mGenerated;
};


extern void evolve(Individual& individual);


#endif // __INDIVIDUAL_H_

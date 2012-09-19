// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <QImage>
#include <QPainter>
#include <QtCore/QDebug>
#include "dna.h"


class Individual {
public:
    Individual(void)
        : mOriginal(NULL)
        , mFitness(std::numeric_limits<quint64>::max())
    { /* ... */ }

    Individual(DNA dna, const QImage& original)
        : mDNA(dna)
        , mOriginal(&original)
        , mFitness(std::numeric_limits<quint64>::max())
        , mGenerated(original.size(), original.format())
    { /* ... */ }

    inline const QImage& generated(void) const { return mGenerated; }
    inline const DNA& dna(void) const { return mDNA; }
    inline quint64 fitness(void) const { return mFitness; }
    inline void operator()(Individual& individual) { individual.evolve(); }

    inline unsigned int rgbDelta(QRgb c1, QRgb c2) {
        const int r = qRed(c1) - qRed(c2);
        const int g = qGreen(c1) - qGreen(c2);
        const int b = qBlue(c1) - qBlue(c2);
        return r*r + g*g + b*b;
    }

    inline void draw(void) {
        QPainter p(&mGenerated);
        p.setPen(Qt::transparent);
        p.setBrush(Qt::white);
        p.drawRect(0, 0, mGenerated.width(), mGenerated.height());
        p.setRenderHint(QPainter::Antialiasing);
        p.scale(mGenerated.width(), mGenerated.height());
        for (DNAType::const_iterator gene = mDNA.constBegin(); gene != mDNA.constEnd(); ++gene) {
            p.setBrush(gene->color());
            p.drawPolygon(gene->polygon());
        }
    }

    inline quint64 calcFitness(void) {
        draw();
        mFitness = 0;
        const QRgb* o = reinterpret_cast<const QRgb*>(mOriginal->bits());
        const QRgb* const oEnd = o + mOriginal->width() * mOriginal->height();
        const QRgb* g = reinterpret_cast<const QRgb*>(mGenerated.bits());
        while (o < oEnd)
            mFitness += rgbDelta(*o++, *g++);
        return mFitness;
    }

    inline void evolve(void) {
        mDNA.mutate();
        calcFitness();
    }

private:
    DNA mDNA;
    const QImage* mOriginal;
    quint64 mFitness;
    QImage mGenerated;
};


#endif // __INDIVIDUAL_H_

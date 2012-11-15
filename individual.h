// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <QImage>
#include <QPainter>
#include <QtCore/QDebug>
#include "helper.h"
#include "dna.h"
#include "breedersettings.h"


class Individual {
public:
    explicit Individual(void)
        : mFitness(std::numeric_limits<quint64>::max())
    { /* ... */ }

    explicit Individual(DNA dna, const QImage& original)
        : mDNA(dna)
        , mOriginal(original)
        , mFitness(std::numeric_limits<quint64>::max())
        , mGenerated(original.size(), original.format())
    { /* ... */ }

    inline const QImage& generated(void) const { return mGenerated; }
    inline const DNA& dna(void) const { return mDNA; }
    inline quint64 fitness(void) const { return mFitness; }
    inline void operator()(Individual& individual) { individual.evolve(); }

    inline void draw(void) {
        if (mGenerated.isNull())
            return;
        QPainter p(&mGenerated);
        p.setPen(Qt::transparent);
        p.setBrush(QBrush(QColor(gSettings.backgroundColor())));
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
        const QRgb* o = reinterpret_cast<const QRgb*>(mOriginal.constBits());
        const QRgb* const oEnd = o + mOriginal.width() * mOriginal.height();
        const QRgb* g = reinterpret_cast<const QRgb*>(mGenerated.constBits());
        while (o < oEnd)
            mFitness += rgbDelta(*o++, *g++);
        return mFitness;
    }

    quint64 maximumFitnessDelta(void) const {
        quint64 maxDelta = 0;
        const QRgb* o = reinterpret_cast<const QRgb*>(mOriginal.constBits());
        const QRgb* const oEnd = o + mOriginal.width() * mOriginal.height();
        while (o < oEnd)
            maxDelta += rgbDelta(*o++, qRgba(255, 255, 255, 255));
        return maxDelta;
    }

    inline void evolve(void) {
        mDNA.mutate();
        calcFitness();
    }

private:
    DNA mDNA;
    QImage mOriginal;
    quint64 mFitness;
    QImage mGenerated;

private: // methods
    static inline unsigned int rgbDelta(QRgb c1, QRgb c2) {
        return square(qRed(c1) - qRed(c2)) + square(qGreen(c1) - qGreen(c2)) + square(qBlue(c1) - qBlue(c2));
    }
};


#endif // __INDIVIDUAL_H_

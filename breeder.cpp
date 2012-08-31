// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breeder.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QtCore/QDebug>
#include <limits>

Breeder::Breeder(QThread* parent)
    : QThread(parent)
    , mMutationRate(100)
{
    reset();
}


void Breeder::setOriginalImage(const QImage& original)
{
    mOriginal = original;
    mGenerated = QImage(mOriginal.size(), mOriginal.format());
    reset();
}


void Breeder::reset(void)
{
    mGeneration = 0;
    mFitness = ULONG_MAX;
    mSelected = 0;
    mDirty = false;
    mStopped = false;
    mDNA.clear();
    mMutation.clear();
    for (int i = 0; i < MAX_GENOMES; ++i)
        mDNA.append(Genome(this));
}


void Breeder::setDeltaR(int d)
{
    mdR = d;
}


void Breeder::setDeltaG(int d)
{
    mdG = d;
}


void Breeder::setDeltaB(int d)
{
    mdB = d;
}


void Breeder::setDeltaA(int d)
{
    mdA = d;
}


void Breeder::setDeltaXY(int d)
{
    mdXY = 1e-3 * (qreal) d;
}


void Breeder::setMutationRate(int r)
{
    mMutationRate = r;
}


inline unsigned long Breeder::deltaE(QRgb c1, QRgb c2)
{
    const unsigned int r = qRed(c1) - qRed(c2);
    const unsigned int g = qGreen(c1) - qGreen(c2);
    const unsigned int b = qBlue(c1) - qBlue(c2);
    return r*r + g*g + b*b;
}


unsigned long Breeder::fitness(void)
{
    Q_ASSERT(mOriginal.size() == mGenerated.size());
    unsigned long sum = 0;
#pragma omp parallel for reduction(+ : sum)
    for (int y = 0; y < mOriginal.height(); ++y) {
        const QRgb* o = reinterpret_cast<QRgb*>(mOriginal.scanLine(y));
        const QRgb* g = reinterpret_cast<QRgb*>(mGenerated.scanLine(y));
        for (int x = 0; x < mOriginal.width(); ++x)
            sum += deltaE(*o++, *g++);
    }
    return sum;
}



void Breeder::draw(void)
{
    QPainter p(&mGenerated);
    p.setPen(Qt::transparent);
    p.setBrush(Qt::white);
    p.drawRect(0, 0, mGenerated.width(), mGenerated.height());
    p.setRenderHint(QPainter::Antialiasing);
    p.scale(mGenerated.width(), mGenerated.height());
    for (DNAType::const_iterator genome = mMutation.constBegin(); genome != mMutation.constEnd(); ++genome) {
        p.setBrush(genome->color());
        p.drawConvexPolygon(genome->polygon());
    }
}


void Breeder::mutate(void)
{
    mMutation = mDNA;
    // TODO: kill/add genomes occasionally
    // ...
    for (DNAType::iterator genome = mMutation.begin(); genome != mMutation.end(); ++genome)
        genome->mutate();
}


void Breeder::proceed(void)
{
    if (mOriginal.isNull() || mGenerated.isNull())
        return;
    mutate();
    draw();
    const unsigned long f = fitness();
    if (f < mFitness) {
        ++mSelected;
        mFitness = f;
        mDNA = mMutation;
        emit evolved();
    }
    ++mGeneration;
    emit proceeded();
}


void Breeder::stop(void)
{
    mStopped = true;
}


void Breeder::breed(void)
{
    mStopped = false;
    mDirty = true;
    start();
}


void Breeder::run(void)
{
    while (!mStopped) {
        proceed();
    }
}

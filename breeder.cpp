// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breeder.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore/QDebug>
#include <limits>

Breeder::Breeder(QThread* parent)
    : QThread(parent)
    , mMutationRate(1)
    , mMinGenomes(MIN_GENOMES)
    , mMaxGenomes(MAX_GENOMES)
{
    reset();
    mRandom.seed(QDateTime::currentDateTime().toTime_t());
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
    int N = mMinGenomes + random() % (mMaxGenomes - mMinGenomes);
    for (int i = 0; i < N; ++i)
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


void Breeder::setDNA(DNA dna)
{
    bool wasRunning = isRunning();
    if (wasRunning)
        stop();
    mDNA = dna;
    if (wasRunning)
        breed();
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
        p.drawPolygon(genome->polygon());
    }
}


inline bool Breeder::willMutate(void) {
    return (random() % mMutationRate) == 0;
}


void Breeder::mutate(void)
{
    mMutation = mDNA;
    if (willMutate() && mMutation.size() < MAX_GENOMES) {
        mMutation.append(Genome(this));
    }
    if (willMutate() && mMutation.size() > MIN_GENOMES) {
        mMutation.remove(random() % mMutation.size());
    }
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
    wait();
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

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breeder.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore/QDebug>

Breeder::Breeder(QThread* parent)
    : QThread(parent)
{
    reset();
}


void Breeder::setOriginalImage(const QImage& original)
{
    mOriginal = original;
    mGenerated = QImage(mOriginal.size(), mOriginal.format());
    reset();
}


void Breeder::setDirty(bool dirty)
{
    mDirty = dirty;
}


void Breeder::setDNA(DNA dna)
{
    const bool wasRunning = isRunning();
    if (wasRunning)
        stop();
    mDNAMutex.lock();
    mDNA = dna;
    mDNAMutex.unlock();
    if (wasRunning)
        breed();
}


void Breeder::reset(void)
{
    mGeneration = 0;
    mFitness = ULONG_MAX;
    mSelected = 0;
    mDirty = false;
    mStopped = false;
    mMutation.clear();
    mDNAMutex.lock();
    mDNA.clear();
    mDNAMutex.unlock();
    populate();
}


void Breeder::populate(void)
{
    QMutexLocker locker(&mDNAMutex);
    int N = gBreederSettings.minGenomes() + MT::random() % (gBreederSettings.maxGenomes() - gBreederSettings.minGenomes());
    for (int i = 0; i < N; ++i)
        mDNA.append(Genome());
}


inline unsigned long Breeder::deltaE(QRgb c1, QRgb c2)
{
    const int r = qRed(c1) - qRed(c2);
    const int g = qGreen(c1) - qGreen(c2);
    const int b = qBlue(c1) - qBlue(c2);
    return r*r + g*g + b*b;
}


inline unsigned long Breeder::fitness(void)
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


inline void Breeder::draw(void)
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


inline bool Breeder::willMutate(unsigned int rate) {
    return (MT::random() % rate) == 0;
}


inline void Breeder::mutate(void)
{
    mMutation = mDNA;
    if (willMutate(gBreederSettings.genomeEmergenceProbability()) && mMutation.size() < gBreederSettings.maxGenomes()) {
        mMutation.append(Genome());
    }
    if (willMutate(gBreederSettings.genomeKillProbability()) && mMutation.size() > gBreederSettings.minGenomes()) {
        mMutation.remove(MT::random() % mMutation.size());
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
        mFitness = f;
        mDNAMutex.lock();
        mDNA = mMutation;
        mDNAMutex.unlock();
        ++mSelected;
        emit evolved(mGenerated, mDNA, mFitness, mSelected, mGeneration+1);
    }
    ++mGeneration;
    emit proceeded(mGeneration);
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

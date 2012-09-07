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
{
    // reset();
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


const DNA& Breeder::dna(void)
{
    QMutexLocker locker(&mDNAMutex);
    return mDNA;
}


void Breeder::setDNA(DNA dna)
{
    bool wasRunning = isRunning();
    if (wasRunning)
        stop();
    mDNAMutex.lock();
    mDNA = dna;
    mMutation = dna;
    mDNAMutex.unlock();
    mFitness = ULONG_MAX;
    draw();
    if (wasRunning)
        breed(priority());
}


void Breeder::setGeneration(unsigned long generation)
{
    qDebug() << "Breeder::setGeneration(" << generation << ")";
    mGeneration = generation;
}


void Breeder::setSelected(unsigned long selected)
{
    qDebug() << "Breeder::setSelected(" << selected << ")";
    mSelected = selected;
}


void Breeder::reset(void)
{
    mGeneration = 0;
    mFitness = ULONG_MAX;
    mSelected = 0;
    mDirty = false;
    mStopped = false;
    mDNAMutex.lock();
    mDNA.clear();
    mDNAMutex.unlock();
    populate();
    mMutation = mDNA;
    draw();
}


void Breeder::populate(void)
{
    QMutexLocker locker(&mDNAMutex);
    for (int i = 0; i < gBreederSettings.minGenomes(); ++i)
        mDNA.append(Genome());
}


inline unsigned long Breeder::deltaE(QRgb c1, QRgb c2)
{
    const int r = qRed(c1) - qRed(c2);
    const int g = qGreen(c1) - qGreen(c2);
    const int b = qBlue(c1) - qBlue(c2);
    return r*r + g*g + b*b;
    /* Farbvergleich über HSV führt zu flauen, schlecht konvergierenden Bildern
//    const QColor& o = QColor(c1).toHsv();
//    const QColor& g = QColor(c2).toHsv();
//    const int h = o.hue() - g.hue();
//    const int s = o.saturation() - g.saturation();
//    const int v = o.value() - g.value();
//    return h*h + s*s + v*v;
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
    // maybe spawn a new genome
    if (willMutate(gBreederSettings.genomeEmergenceProbability()) && mMutation.size() < gBreederSettings.maxGenomes())
        mMutation.append(Genome());
    // maybe kill a genome
    if (willMutate(gBreederSettings.genomeKillProbability()) && mMutation.size() > gBreederSettings.minGenomes())
        mMutation.remove(MT::random() % mMutation.size());
    if (willMutate(gBreederSettings.genomeMoveProbability())) {
        const int oldIndex = MT::random() % mMutation.size();
        const int newIndex = MT::random() % mMutation.size();
        if (oldIndex != newIndex) {
            const Genome genome = mMutation.at(oldIndex);
            mMutation.remove(oldIndex);
            mMutation.insert(newIndex, genome);
            qDebug() << "MOVE" << mMutation.size();
        }
    }
    // mutate all contained genomes
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
        mDirty = true;
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


void Breeder::breed(QThread::Priority priority)
{
    mStopped = false;
    start(priority);
}


void Breeder::run(void)
{
    while (!mStopped) {
        proceed();
    }
}

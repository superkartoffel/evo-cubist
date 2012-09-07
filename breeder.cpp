// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breeder.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore/QDebug>
#include <limits>
#include <qmath.h>

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
    mGeneration = generation;
}


void Breeder::setSelected(unsigned long selected)
{
    mSelected = selected;
}


void Breeder::reset(void)
{
    mGeneration = 0;
    mFitness = ULONG_MAX;
    mSelected = 0;
    mDirty = false;
    mStopped = false;
    populate();
    mMutation = mDNA;
    draw();
}


void Breeder::populate(void)
{
    QMutexLocker locker(&mDNAMutex);
    mDNA.clear();
    qDebug() << "gBreederSettings.startDistribution() =" << gBreederSettings.startDistribution();
    switch (gBreederSettings.startDistribution()) {
    case 0: // random
    {
        qDebug() << "making random start distribution ...";
        for (int i = 0; i < gBreederSettings.minGenomes(); ++i)
            mDNA.append(Genome(true));
        break;
    }
    case 1: // tiled
        // fall-through
    case 2: // tiled with color hint
    {
        qDebug() << "making tiled start distribution ...";
        const qreal N = qSqrt(gBreederSettings.minGenomes() + (gBreederSettings.maxGenomes() - gBreederSettings.minGenomes()) / 2);
        const qreal stepX = 1.0 / N;
        const qreal stepY = 1.0 / N;
        for (qreal y = 0; y < 1.0; y += stepY) {
            for (qreal x = 0; x < 1.0; x += stepX) {
                QPolygonF polygon;
                polygon << QPointF(x, y) << QPointF(x +  stepX, y) << QPointF(x + stepX, y + stepY) << QPointF(x, y + stepY);
                QColor color;
                if (gBreederSettings.startDistribution() == 1) {
                    color = QColor(MT::random() % 255, MT::random() % 255, MT::random() % 255, gBreederSettings.minA() + MT::random() % (gBreederSettings.maxA() - gBreederSettings.minA()));
                }
                else {
                    const int px = (int)(x * mOriginal.width());
                    const int py = (int)(y * mOriginal.height());
                    color = QColor(mOriginal.pixel(px, py));
                    color.setAlpha(color.alpha() % gBreederSettings.maxA());
                }
                mDNA.append(Genome(polygon, color));
            }
        }
        break;
    }
    default:
        qWarning() << "bad start distribution:" << gBreederSettings.startDistribution();
        break;
    }
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
            qDebug() << "MOVE " << oldIndex << ">" << newIndex;
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

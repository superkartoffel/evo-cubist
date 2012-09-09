// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breeder.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore>
#include <QtCore/QDebug>
#include <QVector>
#include <limits>
#include <qmath.h>
#include "individual.h"

Breeder::Breeder(QThread* parent)
    : QThread(parent)
{
    /*...*/
}


void Breeder::setOriginalImage(const QImage& original)
{
    mOriginal = original;
    mGenerated = QImage(mOriginal.size(), mOriginal.format());
    mDNA.setScale(mOriginal.size());
    reset();
}


void Breeder::setDirty(bool dirty)
{
    mDirty = dirty;
}


DNA Breeder::dna(void)
{
    return mDNA;
}


void Breeder::setDNA(DNA dna)
{
    bool wasRunning = isRunning();
    if (wasRunning)
        stop();
    mDNA = dna;
    mMutation = dna;
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
    draw();
}


void Breeder::populate(void)
{
    mDNA.clear();
    switch (gBreederSettings.startDistribution()) {
    case 0: // random
    {
        for (int i = 0; i < gBreederSettings.minGenomes(); ++i)
            mDNA.append(Genome(true));
        break;
    }
    case 1: // tiled
        // fall-through
    case 2: // tiled with color hint
    {
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
    case 3: // scattered
        // fall-through
    case 4: // scattered with color hint
    {
        for (int i = 0; i < gBreederSettings.minGenomes(); ++i) {
            QPolygonF polygon;
            const QPointF mid(MT::random1(), MT::random1());
            for (int j = 0; j < gBreederSettings.minPointsPerGenome(); ++j) {
                const qreal xoff = (MT::random1()-0.5) / (gBreederSettings.scatterFactor() * gBreederSettings.minPointsPerGenome());
                const qreal yoff = (MT::random1()-0.5) / (gBreederSettings.scatterFactor() * gBreederSettings.minPointsPerGenome());
                polygon << (mid + QPointF(xoff, yoff));
            }
            QColor color;
            if (gBreederSettings.startDistribution() == 3) {
                color = QColor(MT::random() % 255, MT::random() % 255, MT::random() % 255, gBreederSettings.minA() + MT::random() % (gBreederSettings.maxA() - gBreederSettings.minA()));
            }
            else {
                const int px = (int)(mid.x() * mOriginal.width());
                const int py = (int)(mid.y() * mOriginal.height());
                color = QColor(mOriginal.pixel(px, py));
                color.setAlpha(color.alpha() % gBreederSettings.maxA());
            }
            mDNA.append(Genome(polygon, color));
        }
        break;
    }
    default:
        qWarning() << "unknown start distribution:" << gBreederSettings.startDistribution();
        break;
    }
    mMutation = mDNA;
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
    // generate N mutations
    while (!mStopped) {
        const int N = gBreederSettings.cores();
        QVector<Individual> population(N);
        for (int i = 0; i < N; ++i)
            population[i] = Individual(mDNA, mOriginal);
        QtConcurrent::blockingMap(population, Individual());
        // find fittest mutation
        QVector<Individual>::iterator best = NULL;
        for (QVector<Individual>::iterator mutation = population.begin(); mutation != population.end(); ++mutation) {
            if (mutation->fitness() < mFitness)
                best = mutation;
        }
        // select fittest mutation if any
        if (best) {
            mFitness = best->fitness();
            mDNA = best->dna();
            mGenerated = best->generated();
            mDirty = true;
            emit evolved(mGenerated, mDNA, mFitness, ++mSelected, mGeneration+N);
        }
        mGeneration += N;
        emit proceeded(mGeneration);
    }
}

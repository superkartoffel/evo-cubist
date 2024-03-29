// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore>
#include <QtCore/QDebug>
#include <QVector>
#include <QMutexLocker>
#include <limits>
#include <qmath.h>
#include "breeder.h"
#include "individual.h"
#include "random/rnd.h"


Breeder::Breeder(QThread* parent)
    : QThread(parent)
    , mDirty(false)
    , mStopped(true)
    , mMaximumFitnessDelta(std::numeric_limits<quint64>::max())
{
    /*...*/
}


void Breeder::setOriginalImage(const QImage& original)
{
    mOriginal = original.convertToFormat(QImage::Format_ARGB32);;
    mGenerated = QImage(mOriginal.size(), mOriginal.format());
    mDNA.setScale(mOriginal.size());
    mMaximumFitnessDelta = Individual(mDNA, mOriginal).maximumFitnessDelta();
    reset();
}


void Breeder::setDirty(bool dirty)
{
    mDirty = dirty;
}


void Breeder::generate(void)
{
    Individual individual(mDNA, mOriginal);
    mFitness = individual.calcFitness();
    mGenerated = individual.generated();
}


void Breeder::setDNA(const DNA& dna)
{
    QMutexLocker locker(&mMutex);
    mDNA = mMutation = dna;
    mGeneration = mSelectedGenerations = dna.generation();
    mSelected = dna.selected();
    mTotalSeconds = dna.totalSeconds();
    generate();
}


void Breeder::setGeneration(unsigned long generation)
{
    mGeneration = mSelectedGenerations = generation;
}


void Breeder::setSelected(unsigned long selected)
{
    mSelected = selected;
}


void Breeder::spliceAt(const QPointF& p)
{
    if (gSettings.minPointsPerGene() > 3)
        return;
    QMutexLocker locker(&mMutex);
    int i = mDNA.size();
    while (i--) {
        const Gene gene = mDNA.at(i);
        if (gene.polygon().containsPoint(p, Qt::OddEvenFill)) {
            QVector<Gene> offsprings =  gene.splice();
            if (offsprings.size() > 0) {
                mDNA[i] = offsprings.first();
                for (int j = 1; j < offsprings.size(); ++j)
                    mDNA.insert(i, offsprings.at(j));
                emit spliced(gene, offsprings);
                mMutation = mDNA;
                generate();
                emit evolved(mGenerated, mDNA, mFitness, mSelected, mSelectedGenerations);
            }
            break;
        }
    }
}


void Breeder::reset(void)
{
    mGeneration = mSelectedGenerations = mSelected = 1;
    mDirty = mStopped = false;
    mTotalSeconds = 0;
    populate();
    generate();
    emit evolved(mGenerated, mDNA, mFitness, mSelected, mSelectedGenerations);
    emit proceeded(mGeneration);
}


void Breeder::populate(void)
{
    QMutexLocker locker(&mMutex);
    mDNA.clear();
    switch (gSettings.startDistribution()) {
    case RandomDistribution:
    {
        for (int i = 0; i < gSettings.minGenes(); ++i)
            mDNA.append(Gene(true));
        break;
    }
    case TiledDistribution:
        // fall-through
    case TiledWithColorHintDistribution:
        // fall-through
    case TiledTrianglesWithColorHintDistribution:
    {
        const int N = qFloor(qSqrt(((gSettings.startDistribution() == TiledTrianglesWithColorHintDistribution)? 0.5 : 1) * gSettings.minGenes()));
        const qreal stepX = 1.0 / N;
        const qreal stepY = 1.0 / N;
        for (qreal y = 0; y < 1.0; y += stepY) {
            for (qreal x = 0; x < 1.0; x += stepX) {
                QColor color;
                if (gSettings.startDistribution() == TiledWithColorHintDistribution || gSettings.startDistribution() == TiledTrianglesWithColorHintDistribution) {
                    const int px = (int)((x + stepX/2) * mOriginal.width());
                    const int py = (int)((y + stepY/2) * mOriginal.height());
                    if (px < mOriginal.width() && py < mOriginal.height()) {
                        color = QColor(mOriginal.pixel(px, py));
                        color.setAlpha(RAND::rnd(gSettings.minA(), gSettings.maxA()));
                    }
                }
                else {
                    color = QColor(RAND::rnd(256), RAND::rnd(256), RAND::rnd(256), RAND::rnd(gSettings.minA(), gSettings.maxA()));
                }
                QPolygonF polygon;
                if (gSettings.startDistribution() == TiledTrianglesWithColorHintDistribution) {
                    polygon << QPointF(x, y) << QPointF(x + stepX, y) << QPointF(x + stepX, y + stepY);
                    QPolygonF polygon2;
                    polygon2 << QPointF(x, y) << QPointF(x, y + stepY) << QPointF(x + stepX, y + stepY);
                    mDNA.append(Gene(polygon2, color));
                }
                else {
                    polygon << QPointF(x, y) << QPointF(x + stepX, y) << QPointF(x + stepX, y + stepY) << QPointF(x, y + stepY);
                }
                mDNA.append(Gene(polygon, color));
            }
        }
        break;
    }
    case ScatteredDistribution:
        // fall-through
    case ScatteredWithColorHintDistribution:
    {
        for (int i = 0; i < gSettings.minGenes(); ++i) {
            QPolygonF polygon;
            const QPointF mid(RAND::rnd1(), RAND::rnd1());
            for (int j = 0; j < gSettings.minPointsPerGene(); ++j) {
                const qreal xoff = RAND::rnd1(-0.5, 0.5) / (gSettings.scatterFactor() * gSettings.minPointsPerGene());
                const qreal yoff = RAND::rnd1(-0.5, 0.5) / (gSettings.scatterFactor() * gSettings.minPointsPerGene());
                polygon << (mid + QPointF(xoff, yoff));
            }
            QColor color;
            if (gSettings.startDistribution() == ScatteredWithColorHintDistribution) {
                const int px = (int)(mid.x() * mOriginal.width());
                const int py = (int)(mid.y() * mOriginal.height());
                if (px < mOriginal.width() && py < mOriginal.height()) {
                    color = QColor(mOriginal.pixel(px, py));
                    color.setAlpha(RAND::rnd(gSettings.minA(), gSettings.maxA()));
                }
            }
            else {
                color = QColor(RAND::rnd(256), RAND::rnd(256), RAND::rnd(256), RAND::rnd(gSettings.minA(), gSettings.maxA()));
            }
            if (gSettings.onlyConvex())
                polygon = convexHull(polygon);
            mDNA.append(Gene(polygon, color));
        }
        break;
    }
    default:
        qWarning() << "unknown start distribution:" << gSettings.startDistribution();
        break;
    }
    mMutation = mDNA;
}


void Breeder::stop(void)
{
    mStopped = true;
    wait(500);
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
        mMutex.lock();
        const int N = gSettings.cores();
        QVector<Individual> population(N);
        for (int i = 0; i < N; ++i)
            population[i] = Individual(mDNA, mOriginal);
        QtConcurrent::blockingMap(population, Individual());
        // find fittest mutation
        QVector<Individual>::const_iterator best = NULL;
        for (QVector<Individual>::const_iterator i = population.constBegin(); i != population.constEnd(); ++i) {
            if (i->fitness() < mFitness) {
                best = i;
                mFitness = best->fitness();
            }
        }
        // select fittest mutation if any
        if (best) {
            mFitness = best->fitness();
            mDNA = best->dna();
            mGenerated = best->generated();
            mDirty = true;
            mSelectedGenerations = mGeneration + N;
        }
        mMutex.unlock();
        mGeneration += N;
        emit proceeded(mGeneration);
        if (best)
            emit evolved(mGenerated, mDNA, mFitness, ++mSelected, mSelectedGenerations);
    }
}

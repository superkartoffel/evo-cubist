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
        for (int y = 0; y < mOriginal->height(); ++y) {
            const QRgb* o = reinterpret_cast<const QRgb*>(mOriginal->scanLine(y));
            const QRgb* g = reinterpret_cast<const QRgb*>(mGenerated.scanLine(y));
            for (int x = 0; x < mOriginal->width(); ++x)
                mFitness += rgbDelta(*o++, *g++);
        }
    }

private:
    DNA mDNA;
    const QImage* mOriginal;
    unsigned long mFitness;
    QImage mGenerated;
};


void evolve(Individual& individual) {
    individual.mutate();
}


void Breeder::run(void)
{
    const int N = QThread::idealThreadCount();
    qDebug() << "QThread::idealThreadCount() =" << N;
    // generate N mutations
    QVector<Individual> population(N);
    while (!mStopped) {
        for (int i = 0; i < N; ++i)
            population[i] = Individual(mDNA, mOriginal);
        QtConcurrent::blockingMap(population, evolve);
        // find fittest mutation
        QVector<Individual>::iterator best = NULL;
        for (QVector<Individual>::iterator mutation = population.begin(); mutation != population.end(); ++mutation) {
            if (mutation->fitness() < mFitness)
                best = mutation;
        }
        // select fittest mutation
        if (best) {
            mFitness = best->fitness();
            mDNA = best->dna();
            mGenerated = best->generated();
            ++mSelected;
            mDirty = true;
            emit evolved(mGenerated, mDNA, mFitness, mSelected, mGeneration+N);
        }
        mGeneration += N;
        emit proceeded(mGeneration);
    }
}

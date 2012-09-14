// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QDateTime>
#include <QtCore>
#include <QtCore/QDebug>
#include <QVector>
#include <limits>
#include <qmath.h>
#include "breeder.h"
#include "individual.h"
#include "random/rnd.h"


Breeder::Breeder(QThread* parent)
    : QThread(parent)
{
    /*...*/
}


void Breeder::setOriginalImage(const QImage& original)
{
    mOriginal = original.convertToFormat(QImage::Format_ARGB32);;
    mGenerated = QImage(mOriginal.size(), mOriginal.format());
    mDNA.setScale(mOriginal.size());
    reset();
}


void Breeder::setDirty(bool dirty)
{
    mDirty = dirty;
}


void Breeder::spliceAt(const QPointF& p)
{
    // TODO: pause run()
    int i = mDNA.size();
    while (i--) {
        if (mDNA.at(i).polygon().containsPoint(p, Qt::OddEvenFill)) {
            QVector<Gene> offsprings =  mDNA.at(i).splice();
            if (offsprings.size() > 0) {
                mDNA[i] = offsprings.first();
                for (int j = 1; j < offsprings.size(); ++j)
                    mDNA.insert(i, offsprings.at(j));
            }
            mMutation = mDNA;
            draw();
            emit evolved(mGenerated, mDNA, mFitness, mSelected, mGeneration);
            break;
        }
    }
    // TODO: resume run()
}


void Breeder::setDNA(const DNA& dna)
{
    bool wasRunning = isRunning();
    if (wasRunning)
        stop();
    mDNA = dna;
    mMutation = dna;
    mFitness = dna.fitness();
    mGeneration = dna.generation();
    mSelected = dna.selected();
    mTotalSeconds = dna.totalSeconds();
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
    mGeneration = 1;
    mFitness = std::numeric_limits<quint64>::max();
    mSelected = 1;
    mDirty = false;
    mStopped = false;
    mTotalSeconds = 0;
    populate();
    draw();
    emit evolved(mGenerated, mDNA, mFitness, mSelected, mGeneration);
    emit proceeded(mGeneration);
}


void Breeder::populate(void)
{
    mDNA.clear();
    switch (gBreederSettings.startDistribution()) {
    case 0: // random
    {
        for (int i = 0; i < gBreederSettings.minGenes(); ++i)
            mDNA.append(Gene(true));
        break;
    }
    case 1: // tiled
        // fall-through
    case 2: // tiled with color hint
        // fall-through
    case 5: // tiled trianges with color hint
    {
        const int N = qFloor(qSqrt(gBreederSettings.maxGenes()));
        const qreal stepX = 1.0 / N;
        const qreal stepY = 1.0 / N;
        for (qreal y = 0; y < 1.0; y += stepY) {
            for (qreal x = 0; x < 1.0; x += stepX) {
                QColor color;
                if (gBreederSettings.startDistribution() == 1) {
                    color = QColor(random(256), random(256), random(256), random(gBreederSettings.minA(), gBreederSettings.maxA()));
                }
                else {
                    const int px = (int)((x + stepX/2) * mOriginal.width());
                    const int py = (int)((y + stepY/2) * mOriginal.height());
                    color = QColor(mOriginal.pixel(px, py));
                    color.setAlpha(random(gBreederSettings.minA(), gBreederSettings.maxA()));
                }
                QPolygonF polygon;
                if (gBreederSettings.startDistribution() == 2) {
                    polygon << QPointF(x, y) << QPointF(x + stepX, y) << QPointF(x + stepX, y + stepY) << QPointF(x, y + stepY);
                }
                else {
                    polygon << QPointF(x, y) << QPointF(x + stepX, y) << QPointF(x + stepX, y + stepY);
                    QPolygonF polygon2;
                    polygon2 << QPointF(x, y) << QPointF(x, y + stepY) << QPointF(x + stepX, y + stepY);
                    mDNA.append(Gene(polygon2, color));
                }
                mDNA.append(Gene(polygon, color));
            }
        }
        break;
    }
    case 3: // scattered
        // fall-through
    case 4: // scattered with color hint
    {
        for (int i = 0; i < gBreederSettings.minGenes(); ++i) {
            QPolygonF polygon;
            const QPointF mid(random1(), random1());
            for (int j = 0; j < gBreederSettings.minPointsPerGene(); ++j) {
                const qreal xoff = random1(-0.5, 0.5) / (gBreederSettings.scatterFactor() * gBreederSettings.minPointsPerGene());
                const qreal yoff = random1(-0.5, 0.5) / (gBreederSettings.scatterFactor() * gBreederSettings.minPointsPerGene());
                polygon << (mid + QPointF(xoff, yoff));
            }
            QColor color;
            if (gBreederSettings.startDistribution() == 3) {
                color = QColor(random(256), random(256), random(256), random(gBreederSettings.minA(), gBreederSettings.maxA()));
            }
            else {
                const int px = (int)(mid.x() * mOriginal.width());
                const int py = (int)(mid.y() * mOriginal.height());
                color = QColor(mOriginal.pixel(px, py));
                color.setAlpha(random(gBreederSettings.minA(), gBreederSettings.maxA()));
            }
            mDNA.append(Gene(polygon, color));
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
    for (DNAType::const_iterator gene = mMutation.constBegin(); gene != mMutation.constEnd(); ++gene) {
        p.setBrush(gene->color());
        p.drawPolygon(gene->polygon());
    }
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

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDER_H_
#define __BREEDER_H_

#include <QObject>
#include <QImage>
#include <QPointF>
#include <QRgb>
#include <QThread>
#include <QMutex>
#include <QtCore/QDebug>

#include <limits>

#include "dna.h"
#include "gene.h"
#include "random/mersenne_twister.h"
#include "breedersettings.h"


class Breeder : public QThread
{
    Q_OBJECT

public:
    Breeder(QThread* parent = NULL);
    void reset(void);
    void populate(void);

    DNA dna(void) { return mDNA; }
    const DNA& constDNA(void) const { return mDNA; }
    inline const QImage& image(void) const { return mGenerated; }
    inline const QImage& originalImage(void) const { return mOriginal; }
    inline unsigned long generation(void) const { return mGeneration; }
    inline quint64 currentFitness(void) const { return mFitness; }
    inline unsigned long selected(void) const { return mSelected; }

    QPolygonF findPolygon(const QPointF&, const DNA&) const;

    void breed(QThread::Priority = QThread::LowPriority);
    void stop(void);
    bool isDirty(void) const { return mDirty; }
    void setDNA(const DNA&);
    void setDirty(bool);
    void setGeneration(unsigned long);
    void setSelected(unsigned long);
    void addTotalSeconds(quint64 s) { mTotalSeconds += s; }
    quint64 totalSeconds(void) const { return mTotalSeconds; }

protected:
    virtual void run(void);
    
private:
    void draw(void);

    bool mDirty;
    volatile bool mStopped;
    unsigned long mGeneration;
    quint64 mFitness;
    unsigned long mSelected;
    quint64 mTotalSeconds;
    QImage mOriginal;
    QImage mGenerated;
    DNA mDNA;
    DNA mMutation;
    QMutex mMutex;

signals:
    void evolved(const QImage&, const DNA&, quint64, unsigned long, unsigned long);
    void proceeded(unsigned long);
    void spliced(const Gene& gene, const QVector<Gene>& offsprings);
    
public slots:
    void setOriginalImage(const QImage&);
    void spliceAt(const QPointF&);
};

#endif // __BREEDER_H_


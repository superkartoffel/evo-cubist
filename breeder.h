// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDER_H_
#define __BREEDER_H_

#include <QObject>
#include <QImage>
#include <QRgb>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "dna.h"
#include "genome.h"
#include "random/mersenne_twister.h"
#include "breedersettings.h"

#include <QtOpenGL>


class Breeder : public QThread
{
    Q_OBJECT

public:
    Breeder(QThread* parent = NULL);
    void reset(void);
    void populate(void);
    void mutate(void);

    inline const DNA& dna(void) {
        QMutexLocker locker(&mDNAMutex);
        return mDNA;
    }

    inline const QImage& image(void) const { return mGenerated; }
    inline const QImage& originalImage(void) const { return mOriginal; }
    inline unsigned long generation(void) const { return mGeneration; }
    inline unsigned long currentFitness(void) const { return mFitness; }
    inline unsigned long selected(void) const { return mSelected; }

    void proceed(void);
    void breed(void);
    void stop(void);
    bool isDirty(void) const { return mDirty; }
    void setDNA(DNA);
    void setDirty(bool);

    QMutex* dnaMutex(void) { return &mDNAMutex; }

protected:
    virtual void run(void);
    
private:
    unsigned long fitness(void);
    static unsigned long deltaE(QRgb c1, QRgb c2);
    void draw(void);
    bool willMutate(unsigned int rate);

    bool mDirty;
    bool mStopped;
    unsigned long mGeneration;
    unsigned long mFitness;
    unsigned long mSelected;
    QImage mOriginal;
    QImage mGenerated;
    DNA mDNA;
    DNA mMutation;

    QMutex mDNAMutex;

signals:
    void evolved(const QImage&, const DNA&, unsigned int, unsigned int, unsigned int);
    void proceeded(unsigned int);
    
public slots:
    void setOriginalImage(const QImage&);
};

#endif // __BREEDER_H_


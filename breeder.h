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

#include <limits>

#include "dna.h"
#include "genome.h"
#include "random/mersenne_twister.h"
#include "optionsform.h"

using namespace randomtools;


class Breeder : public QThread
{
    Q_OBJECT

public:
    friend class Genome;

    Breeder(QThread* parent = NULL);
    void setOptions(const OptionsForm* options);
    void reset(void);
    void populate(void);
    void mutate(void);

    const DNA& dna(void);
    inline const QImage& image(void) const { return mGenerated; }
    inline const QImage& originalImage(void) const { return mOriginal; }
    inline unsigned long generation(void) const { return mGeneration; }
    inline unsigned long currentFitness(void) const { return mFitness; }
    inline unsigned long selected(void) const { return mSelected; }
    inline unsigned int random(void) { return mRandom.next(); }
    inline qreal random1(void) { return (qreal)random() / mRandom.max(); }

    void proceed(void);
    void breed(void);
    void stop(void);
    bool isDirty(void) const { return mDirty; }
    void setDNA(DNA);

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

    int mdR;
    int mdG;
    int mdB;
    int mdA;
    qreal mdXY;

    static const int MIN_GENOMES = 200;
    static const int MAX_GENOMES = 400;

    MersenneTwister mRandom;
    QMutex mDNAMutex;
    const OptionsForm* mOptions;

signals:
    void evolved(const QImage&, const DNA&, unsigned int, unsigned int, unsigned int);
    void proceeded(unsigned int);
    
public slots:
    void setOriginalImage(const QImage&);
    void setDeltaR(int);
    void setDeltaG(int);
    void setDeltaB(int);
    void setDeltaA(int);
    void setDeltaXY(int);
};

#endif // __BREEDER_H_


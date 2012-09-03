// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDER_H_
#define __BREEDER_H_

#include <QObject>
#include <QImage>
#include <QRgb>
#include <QThread>

#include <limits>

#include "dna.h"
#include "genome.h"
#include "random/mersenne_twister.h"

using namespace randomtools;


class Breeder : public QThread
{
    Q_OBJECT

public:
    friend class Genome;

    Breeder(QThread* parent = NULL);
    void reset(void);
    void populate(void);
    void mutate(void);

    inline const DNA& dna(void) const { return mDNA; }
    inline const DNA& mutation(void) const { return mMutation; }
    inline const QImage& image(void) const { return mGenerated; }
    inline const QImage& originalImage(void) const { return mOriginal; }
    inline unsigned long generation(void) const { return mGeneration; }
    inline unsigned long currentFitness(void) const { return mFitness; }
    inline unsigned long selected(void) const { return mSelected; }
    unsigned int random(void) { return mRandom.next(); }
    qreal random1(void) { return (qreal)mRandom.next() / mRandom.max(); }

    void proceed(void);
    void breed(void);
    void stop(void);
    bool isDirty(void) const { return mDirty; }
    void setDNA(DNA);

protected:
    virtual void run(void);
    
private:
    unsigned long fitness(void);
    static unsigned long deltaE(QRgb c1, QRgb c2);
    void draw(void);
    bool willMutate(void);

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
    int mMutationRate;
    int mMinGenomes;
    int mMaxGenomes;

    static const int MIN_GENOMES = 200;
    static const int MAX_GENOMES = 400;

    MersenneTwister mRandom;

signals:
    void evolved(void);
    void proceeded(void);
    
public slots:
    void setOriginalImage(const QImage&);
    void setDeltaR(int);
    void setDeltaG(int);
    void setDeltaB(int);
    void setDeltaA(int);
    void setDeltaXY(int);
    void setMutationRate(int);

};

#endif // __BREEDER_H_


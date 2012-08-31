// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDER_H_
#define __BREEDER_H_

#include <QObject>
#include <QImage>
#include <QRgb>
#include <QThread>

#include "dna.h"
#include "genome.h"

class Breeder : public QThread
{
    Q_OBJECT

public:
    friend class Genome;

    explicit Breeder(QThread* parent = NULL);
    void reset(void);
    void mutate(void);
    const DNA& dna(void) const { return mDNA; }
    const DNA& mutation(void) const { return mMutation; }
    const QImage& image(void) const { return mGenerated; }
    unsigned long generation(void) const { return mGeneration; }
    unsigned long currentFitness(void) const { return mFitness; }
    unsigned long selected(void) const { return mSelected; }
    void breed(void);
    void stop(void);
    bool isDirty(void) const { return mDirty; }

    static const int MutationRate = 1500;

protected:
    virtual void run(void);
    
private:
    unsigned long fitness(void);
    static unsigned long deltaE(QRgb c1, QRgb c2);
    void draw(void);
    void proceed(void);

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

    static const int MAX_GENOMES = 200;

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


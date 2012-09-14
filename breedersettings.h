// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDERSETTINGS_H_
#define __BREEDERSETTINGS_H_

#include <QObject>

class BreederSettings : public QObject
{
    Q_OBJECT

public:
    BreederSettings::BreederSettings(void)
        : QObject(NULL)
        , mdXY(0.5)
        , mdR(128)
        , mdG(128)
        , mdB(128)
        , mdA(128)
        , mMinA(10)
        , mMaxA(60)
        , mColorMutationProbability(700)
        , mPointMutationProbability(700)
        , mPointKillProbability(700)
        , mPointEmergenceProbability(700)
        , mGeneKillProbability(700)
        , mGeneMoveProbability(700)
        , mGeneSliceProbability(7000)
        , mGeneEmergenceProbability(700)
        , mMinPointsPerGene(3)
        , mMaxPointsPerGene(9)
        , mMinGenes(200)
        , mMaxGenes(400)
        , mStartDistribution(0)
        , mScatterFactor(0.5)
        , mAutoSaveInterval(10)
        , mCores(2)
    {
        // ...
    }

    double dXY(void) const { return mdXY; }
    inline int dR(void) const { return mdR; }
    inline int dG(void) const { return mdG; }
    inline int dB(void) const { return mdB; }
    inline int dA(void) const { return mdA; }
    inline int minA(void) const { return mMinA; }
    inline int maxA(void) const { return mMaxA; }
    inline int colorMutationProbability(void) const { return mColorMutationProbability; }
    inline int pointMutationProbability(void) const { return mPointMutationProbability; }
    inline int pointKillProbability(void) const { return mPointKillProbability; }
    inline int pointEmergenceProbability(void) const { return mPointEmergenceProbability; }
    inline int geneKillProbability(void) const { return mGeneKillProbability; }
    inline int geneMoveProbability(void) const { return mGeneMoveProbability; }
    inline int geneSliceProbability(void) const { return mGeneSliceProbability; }
    inline int geneEmergenceProbability(void) const { return mGeneEmergenceProbability; }
    inline int minPointsPerGene(void) const { return mMinPointsPerGene; }
    inline int maxPointsPerGene(void) const { return mMaxPointsPerGene; }
    inline int minGenes(void) const { return mMinGenes; }
    inline int maxGenes(void) const { return mMaxGenes; }
    inline int startDistribution(void) const {  return mStartDistribution; }
    inline qreal scatterFactor(void) const { return mScatterFactor; }
    inline int cores(void) const { return mCores; }

public slots:
    void setDeltaXY(int);
    void setDeltaR(int);
    void setDeltaG(int);
    void setDeltaB(int);
    void setDeltaA(int);
    void setMinA(int);
    void setMaxA(int);
    void setColorMutationProbability(int);
    void setPointMutationProbability(int);
    void setPointKillProbability(int);
    void setPointEmergenceProbability(int);
    void setGeneKillProbability(int);
    void setGeneMoveProbability(int);
    void setGeneSliceProbability(int);
    void setGeneEmergenceProbability(int);
    void setMinPointsPerGene(int);
    void setMaxPointsPerGene(int);
    void setMinGenes(int);
    void setMaxGenes(int);
    void setCores(int);
    void setGPUComputing(bool);
    void setStartDistribution(int);
    void setScatterFactor(double);

private:
    double mdXY; // [0..1)
    int mdR; // [0..255]
    int mdG; // [0..255]
    int mdB; // [0..255]
    int mdA; // [0..255]
    int mMinA; // [0..255]
    int mMaxA; // [0..255]
    int mColorMutationProbability;
    int mPointMutationProbability;
    int mPointKillProbability;
    int mPointEmergenceProbability;
    int mGeneKillProbability;
    int mGeneMoveProbability;
    int mGeneSliceProbability;
    int mGeneEmergenceProbability;
    int mMinPointsPerGene; // [3..n]
    int mMaxPointsPerGene; // [n..n+x]
    int mMinGenes;
    int mMaxGenes;
    int mStartDistribution;
    qreal mScatterFactor;
    int mAutoSaveInterval; // secs
    int mCores;
    bool mGPUComputing;
};


extern BreederSettings gSettings;

#endif // __BREEDERSETTINGS_H_

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDERSETTINGS_H_
#define __BREEDERSETTINGS_H_

#include <QObject>

class BreederSettings : public QObject
{
    Q_OBJECT

public:
    BreederSettings(void);

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
    inline int genomeKillProbability(void) const { return mGenomeKillProbability; }
    inline int genomeMoveProbability(void) const { return mGenomeMoveProbability; }
    inline int genomeEmergenceProbability(void) const { return mGenomeEmergenceProbability; }
    inline int minPointsPerGenome(void) const { return mMinPointsPerGenome; }
    inline int maxPointsPerGenome(void) const { return mMaxPointsPerGenome; }
    inline int minGenomes(void) const { return mMinGenomes; }
    inline int maxGenomes(void) const { return mMaxGenomes; }
    inline int startDistribution(void) const {  return mStartDistribution; }

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
    void setGenomeKillProbability(int);
    void setGenomeMoveProbability(int);
    void setGenomeEmergenceProbability(int);
    void setMinPointsPerGenome(int);
    void setMaxPointsPerGenome(int);
    void setMinGenomes(int);
    void setMaxGenomes(int);
    void setParallelComputing(bool);
    void setGPUComputing(bool);
    void setStartDistribution(int);

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
    int mGenomeKillProbability;
    int mGenomeMoveProbability;
    int mGenomeEmergenceProbability;
    int mMinPointsPerGenome; // [3..n]
    int mMaxPointsPerGenome; // [n..n+x]
    int mMinGenomes;
    int mMaxGenomes;
    int mAutoSaveInterval; // secs
    bool mParallelComputing;
    bool mGPUComputing;
    int mStartDistribution;
};


extern BreederSettings gBreederSettings;

#endif // __BREEDERSETTINGS_H_

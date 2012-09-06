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
    int dR(void) const { return mdR; }
    int dG(void) const { return mdG; }
    int dB(void) const { return mdB; }
    int dA(void) const { return mdA; }
    int minA(void) const { return mMinA; }
    int maxA(void) const { return mMaxA; }
    int colorMutationProbability(void) const { return mColorMutationProbability; }
    int pointMutationProbability(void) const { return mPointMutationProbability; }
    int pointKillProbability(void) const { return mPointKillProbability; }
    int pointEmergenceProbability(void) const { return mPointEmergenceProbability; }
    int genomeKillProbability(void) const { return mGenomeKillProbability; }
    int genomeMoveProbability(void) const { return mGenomeMoveProbability; }
    int genomeEmergenceProbability(void) const { return mGenomeEmergenceProbability; }
    int minPointsPerGenome(void) const { return mMinPointsPerGenome; }
    int maxPointsPerGenome(void) const { return mMaxPointsPerGenome; }
    int minGenomes(void) const { return mMinGenomes; }
    int maxGenomes(void) const { return mMaxGenomes; }

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
};


extern BreederSettings gBreederSettings;

#endif // __BREEDERSETTINGS_H_

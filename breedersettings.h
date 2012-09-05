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
    int colorMutationRate(void) const { return mColorMutationRate; }
    int pointMutationRate(void) const { return mPointMutationRate; }
    int pointKillRate(void) const { return mPointKillRate; }
    int pointEmergenceRate(void) const { return mPointEmergenceRate; }
    int genomeKillRate(void) const { return mGenomeKillRate; }
    int genomeEmergenceRate(void) const { return mGenomeEmergenceRate; }
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
    void setColorMutationRate(int);
    void setPointMutationRate(int);
    void setPointKillRate(int);
    void setPointEmergenceRate(int);
    void setGenomeKillRate(int);
    void setGenomeEmergenceRate(int);
    void setMinPointsPerGenome(int);
    void setMaxPointsPerGenome(int);
    void setMinGenomes(int);
    void setMaxGenomes(int);

private:
    double mdXY; // [0..1)
    int mdR; // [0..255]
    int mdG; // [0..255]
    int mdB; // [0..255]
    int mdA; // [0..255]
    int mMinA; // [0..255]
    int mMaxA; // [0..255]
    int mColorMutationRate;
    int mPointMutationRate;
    int mPointKillRate;
    int mPointEmergenceRate;
    int mGenomeKillRate;
    int mGenomeEmergenceRate;
    int mMinPointsPerGenome; // [3..n]
    int mMaxPointsPerGenome; // [n..n+x]
    int mMinGenomes;
    int mMaxGenomes;
};


extern BreederSettings gBreederSettings;

#endif // __BREEDERSETTINGS_H_

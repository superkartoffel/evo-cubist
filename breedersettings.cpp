// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "breedersettings.h"
#include <QtCore/QDebug>

/// global settings object
BreederSettings gBreederSettings;


void BreederSettings::setDeltaR(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mdR = v;
}


void BreederSettings::setDeltaG(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mdG = v;
}


void BreederSettings::setDeltaB(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mdB = v;
}


void BreederSettings::setDeltaA(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mdA = v;
}


void BreederSettings::setDeltaXY(int v)
{
    mdXY = 1e-4 * v;
    Q_ASSERT(mdXY >= 0.0);
    Q_ASSERT(mdXY < 1.0);
}


void BreederSettings::setMinA(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mMinA = v;
}


void BreederSettings::setMaxA(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    mMaxA = v;
}


void BreederSettings::setColorMutationProbability(int v)
{
    Q_ASSERT(v > 0);
    mColorMutationProbability = v;
}


void BreederSettings::setPointMutationProbability(int v)
{
    Q_ASSERT(v > 0);
    mPointMutationProbability = v;
}


void BreederSettings::setPointKillProbability(int v)
{
    Q_ASSERT(v > 0);
    mPointKillProbability = v;
}


void BreederSettings::setPointEmergenceProbability(int v)
{
    Q_ASSERT(v > 0);
    mPointEmergenceProbability = v;
}


void BreederSettings::setGenomeKillProbability(int v)
{
    Q_ASSERT(v > 0);
    mGenomeKillProbability = v;
}


void BreederSettings::setGenomeMoveProbability(int v)
{
    Q_ASSERT(v > 0);
    mGenomeMoveProbability = v;
}


void BreederSettings::setGenomeEmergenceProbability(int v)
{
    Q_ASSERT(v > 0);
    mGenomeEmergenceProbability = v;
}


void BreederSettings::setMinPointsPerGenome(int v)
{
    Q_ASSERT(v > 2);
    Q_ASSERT(v <= mMaxPointsPerGenome);
    mMinPointsPerGenome = v;
}


void BreederSettings::setMaxPointsPerGenome(int v)
{
    Q_ASSERT(v >= mMinPointsPerGenome);
    mMaxPointsPerGenome = v;
}


void BreederSettings::setMinGenomes(int v)
{
    Q_ASSERT(v > 0);
    mMinGenomes = v;
}


void BreederSettings::setMaxGenomes(int v)
{
    Q_ASSERT(v > 0);
    mMaxGenomes = v;
}


void BreederSettings::setScatterFactor(double v)
{
    Q_ASSERT(v > 0);
    mScatterFactor = v;
}


void BreederSettings::setGPUComputing(bool enabled)
{
    mGPUComputing = enabled;
}


void BreederSettings::setCores(int v)
{
    Q_ASSERT(v > 0);
    mCores = v;
}




void BreederSettings::setStartDistribution(int index)
{
    mStartDistribution = index;
}

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtCore/QDebug>
#include <QString>
#include <QTextStream>
#include "breedersettings.h"

/// global settings object
BreederSettings gSettings;


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


void BreederSettings::setGeneKillProbability(int v)
{
    Q_ASSERT(v > 0);
    mGeneKillProbability = v;
}


void BreederSettings::setGeneMoveProbability(int v)
{
    Q_ASSERT(v > 0);
    mGeneMoveProbability = v;
}


void BreederSettings::setGeneSliceProbability(int v)
{
    Q_ASSERT(v > 0);
    mGeneSliceProbability = v;
}


void BreederSettings::setGeneEmergenceProbability(int v)
{
    Q_ASSERT(v > 0);
    mGeneEmergenceProbability = v;
}


void BreederSettings::setMinPointsPerGene(int v)
{
    Q_ASSERT(v > 2);
    Q_ASSERT(v <= mMaxPointsPerGene);
    mMinPointsPerGene = v;
}


void BreederSettings::setMaxPointsPerGene(int v)
{
    Q_ASSERT(v >= mMinPointsPerGene);
    mMaxPointsPerGene = v;
}


void BreederSettings::setMinGenes(int v)
{
    Q_ASSERT(v > 0);
    mMinGenes = v;
}


void BreederSettings::setMaxGenes(int v)
{
    Q_ASSERT(v > 0);
    mMaxGenes = v;
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


QString BreederSettings::toXml(void) const
{
    QString xml;
    QTextStream out(&xml);
    out << "<settings>\n"
        << "  <deltas>\n"
        << "    <xy>" << mdXY << "</xy>\n"
        << "    <red>" << mdR << "</red>\n"
        << "    <green>" << mdG << "</green>\n"
        << "    <blue>" << mdB << "</blue>\n"
        << "    <alpha>" << mdA << "</alpha>\n"
        << "  </deltas>\n"
        << "  <breeder>\n"
        << "    <minA>" << mMinA << "</minA>\n"
        << "    <maxA>" << mMaxA << "</maxA>\n"
        << "    <colorMutationProbability>" << mColorMutationProbability << "</colorMutationProbability>\n"
        << "    <pointMutationProbability>" << mPointMutationProbability << "</pointMutationProbability>\n"
        << "    <pointKillProbability>" << mPointKillProbability << "</pointKillProbability>\n"
        << "    <pointEmergenceProbability>" << mPointEmergenceProbability << "</pointEmergenceProbability>\n"
        << "    <geneKillProbability>" << mGeneKillProbability << "</geneKillProbability>\n"
        << "    <geneMoveProbability>" << mGeneMoveProbability << "</geneMoveProbability>\n"
        << "    <geneSliceProbability>" << mGeneSliceProbability << "</geneSliceProbability>\n"
        << "    <geneEmergenceProbability>" << mGeneEmergenceProbability << "</geneEmergenceProbability>\n"
        << "    <minPointsPerGene>" << mMinPointsPerGene << "</minPointsPerGene>\n"
        << "    <maxPointsPerGene>" << mMaxPointsPerGene << "</maxPointsPerGene>\n"
        << "    <minGenes>" << mMinGenes << "</minGenes>\n"
        << "    <maxGenes>" << mMaxGenes << "</maxGenes>\n"
        << "    <startDistribution>" << mStartDistribution << "</startDistribution>\n"
        << "    <scatterFactor>" << mScatterFactor << "</scatterFactor>\n"
        << "    <autoSaveInterval>" << mAutoSaveInterval << "</autoSaveInterval>\n"
        << "    <cores>" << mCores << "</cores>\n"
        << "    <gpuComputing>" << mGPUComputing << "</gpuComputing>\n"
        << "  </breeder>\n"
        << "</settings>\n";
    return xml;
}

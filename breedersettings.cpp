// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtCore/QDebug>
#include <QString>
#include <QFile>
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


void BreederSettings::setCurrentDNAFile(const QString& dnaFile)
{
    mCurrentDNAFile = dnaFile;
}


void BreederSettings::setCurrentImageFile(const QString& imageFile)
{
    mCurrentImageFile = imageFile;
}


bool BreederSettings::save(const QString &fileName)
{
    bool rc;
    QFile file(fileName);
    rc = file.open(QIODevice::WriteOnly);
    if (!rc)
        return false;
    QTextStream out(&file);
    out << "<evocubist-settings>\n"
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
        << "    <cores>" << mCores << "</cores>\n"
        << "    <gpuComputing>" << mGPUComputing << "</gpuComputing>\n"
        << "  </breeder>\n"
        << "  <files>\n"
        << "    <dna>" << mCurrentDNAFile << "</dna>\n"
        << "    <image>" << mCurrentImageFile << "</image>\n"
        << "  </files>\n"
        << "  <autosave>\n"
        << "    <imageDirectory>" << mImageSaveDirectory << "</imageDirectory>\n"
        << "    <imageFilenameTemplate>" << mImageSaveFilenameTemplate << "</imageFilenameTemplate>\n"
        << "    <dnaDirectory>" << mDNASaveDirectory << "</dnaDirectory>\n"
        << "    <dnaFilenameTemplate>" << mDNASaveFilenameTemplate << "</dnaFilenameTemplate>\n"
        << "    <interval>" << mAutoSaveInterval << "</interval>\n"
        << "  </autosace>\n"
        << "</evocubist-settings>\n";
    file.close();
    return rc;
}


bool BreederSettings::load(const QString &fileName)
{
    return true;
}


void BreederSettings::readXY(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "xy");
    bool ok = false;
    const QString& xyString = mXml.readElementText();
    const int xy = xyString.toDouble(&ok);
    if (ok)
        mdXY = xy;
    else
        mXml.raiseError(QObject::tr("invalid delta green: %1").arg(xyString));
}

void BreederSettings::readRed(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "red");
    bool ok = false;
    const QString& redString = mXml.readElementText();
    const int r = redString.toInt(&ok);
    if (ok)
        mdR = r;
    else
        mXml.raiseError(QObject::tr("invalid delta green: %1").arg(redString));
}


void BreederSettings::readGreen(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "green");
    bool ok = false;
    const QString& greenString = mXml.readElementText();
    const int g = greenString.toInt(&ok);
    if (ok)
        mdG = g;
    else
        mXml.raiseError(QObject::tr("invalid delta green: %1").arg(greenString));
}


void BreederSettings::readBlue(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "blue");
    bool ok = false;
    const QString& blueString = mXml.readElementText();
    const int b = blueString.toInt(&ok);
    if (ok)
        mdB = b;
    else
        mXml.raiseError(QObject::tr("invalid delta blue: %1").arg(blueString));
}


void BreederSettings::readAlpha(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "alpha");
    bool ok = false;
    const QString& alphaString = mXml.readElementText();
    const int a = alphaString.toInt(&ok);
    if (ok)
        mdA = a;
    else
        mXml.raiseError(QObject::tr("invalid delta alpha: %1").arg(alphaString));
}


void BreederSettings::readDeltas(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "deltas");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "xy") {
            readXY();
        }
        if (mXml.name() == "red") {
            readRed();
        }
        if (mXml.name() == "green") {
            readGreen();
        }
        if (mXml.name() == "blue") {
            readBlue();
        }
        if (mXml.name() == "alpha") {
            readAlpha();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}

void BreederSettings::readMinA(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "minA");

}


void BreederSettings::readMaxA(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxA");

}


void BreederSettings::readColorMutationProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "colorMutationProbability");

}


void BreederSettings::readPointMutationProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointMutationProbability");

}


void BreederSettings::readPointKillProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointKillProbability");

}


void BreederSettings::readPointEmergenceProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointEmergenceProbability");

}


void BreederSettings::readGeneKillProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneKillProbability");

}


void BreederSettings::readGeneMoveProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneMoveProbability");

}


void BreederSettings::readGeneSliceProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneSliceProbability");

}


void BreederSettings::readGeneEmergenceProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneEmergenceProbability");

}


void BreederSettings::readMinPointsPerGene(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "minPointsPerGene");

}


void BreederSettings::readMaxPointsPerGene(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxPointsPerGene");

}


void BreederSettings::readMinGenes(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "minGenes");

}


void BreederSettings::readMaxGenes(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxGenes");

}


void BreederSettings::readStartDistribution(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "startDistribution");

}


void BreederSettings::readScatterFactor(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "scatterFactor");

}


void BreederSettings::readCores(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "cores");

}


void BreederSettings::readGPUComputing(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "gpuComputing");

}


void BreederSettings::readBreeder(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "breeder");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "minA") {
            readMinA();
        }
        if (mXml.name() == "maxA") {
            readMaxA();
        }
        if (mXml.name() == "colorMutationProbability") {
            readColorMutationProbability();
        }
        if (mXml.name() == "pointMutationProbability") {
            readPointMutationProbability();
        }
        if (mXml.name() == "pointKillProbability") {
            readPointKillProbability();
        }
        if (mXml.name() == "pointEmergenceProbability") {
            readPointEmergenceProbability();
        }
        if (mXml.name() == "geneKillProbability") {
            readGeneKillProbability();
        }
        if (mXml.name() == "geneMoveProbability") {
            readGeneMoveProbability();
        }
        if (mXml.name() == "geneSliceProbability") {
            readGeneSliceProbability();
        }
        if (mXml.name() == "geneEmergenceProbability") {
            readGeneEmergenceProbability();
        }
        if (mXml.name() == "minPointsPerGene") {
            readMinPointsPerGene();
        }
        if (mXml.name() == "maxPointsPerGene") {
            readMaxPointsPerGene();
        }
        if (mXml.name() == "minGenes") {
            readMinGenes();
        }
        if (mXml.name() == "maxGenes") {
            readMaxGenes();
        }
        if (mXml.name() == "startDistribution") {
            readStartDistribution();
        }
        if (mXml.name() == "scatterFactor") {
            readScatterFactor();
        }
        if (mXml.name() == "cores") {
            readCores();
        }
        if (mXml.name() == "gpuComputing") {
            readGPUComputing();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}


void BreederSettings::readFiles(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "files");

}



void BreederSettings::readAutoSaveInterval(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "interval");

}


void BreederSettings::readAutosave(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "autosave");

}


void BreederSettings::read(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "evocubist-settings");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "deltas") {
            readDeltas();
        }
        else if (mXml.name() == "breeder") {
            readBreeder();
        }
        else if (mXml.name() == "files") {
            readFiles();
        }
        else if (mXml.name() == "autosave") {
            readAutosave();
        }
        else {
            mXml.skipCurrentElement();
        }
    }

}


bool BreederSettings::read(QIODevice* device)
{
    mXml.setDevice(device);
    if (mXml.readNextStartElement()) {
        if (mXml.name() == "evocubist-settings" && mXml.attributes().value("version").startsWith("1.")) {
            read();
        }
        else {
            mXml.raiseError(QObject::tr("The file is not an Evo Cubist v1.x settings file."));
        }
    }
    return !mXml.error();
}


QString BreederSettings::errorString() const
{
    return QObject::tr("%1 (line %2, column %3)").arg(mXml.errorString()).arg(mXml.lineNumber()).arg(mXml.columnNumber());
}


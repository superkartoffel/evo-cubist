// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtCore/QDebug>
#include <QString>
#include <QFile>
#include <QColor>
#include <QTextStream>
#include <QTextCodec>
#include "breedersettings.h"
#include "main.h"

/// global settings object
BreederSettings gSettings;


void BreederSettings::setBackgroundColor(QRgb v)
{
    mBackgroundColor = v;
}


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


void BreederSettings::setOnlyConvex(bool v)
{
    mOnlyConvex = v;
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


void BreederSettings::setAutoSaveInterval(int v)
{
    Q_ASSERT(v > 0);
    mAutoSaveInterval = v;
}


void BreederSettings::setAutoSave(bool v)
{
    mAutoSave = v;
}


void BreederSettings::setStartDistribution(int index)
{
    mStartDistribution = index;
}


void BreederSettings::setLogFile(const QString& logFile)
{
    mLogFile = logFile;
}


void BreederSettings::setCurrentDNAFile(const QString& dnaFile)
{
    mCurrentDNAFile = dnaFile;
}


void BreederSettings::setCurrentImageFile(const QString& imageFile)
{
    mCurrentImageFile = imageFile;
}


void BreederSettings::setImageSaveDirectory(const QString& v)
{
    mImageSaveDirectory = v;
}


void BreederSettings::setImageSaveFilenameTemplate(const QString& v)
{
    mImageSaveFilenameTemplate = v;
}


void BreederSettings::setDNASaveDirectory(const QString& v)
{
    mDNASaveDirectory = v;
}


void BreederSettings::setDNASaveFilenameTemplate(const QString& v)
{
    mDNASaveFilenameTemplate = v;
}


bool BreederSettings::save(const QString& fileName)
{
    Q_ASSERT(!fileName.isEmpty());
    QFile file(fileName);
    bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok)
        return false;
    QTextStream out(&file);
    out.setAutoDetectUnicode(false);
    out.setCodec(QTextCodec::codecForMib(106/* UTF-8 */));
    const QString& colorString = QString("rgb(%1,%2,%3)")
            .arg(qRed(mBackgroundColor))
            .arg(qGreen(mBackgroundColor))
            .arg(qBlue(mBackgroundColor));
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
        << "<evocubist-settings version=\"" << AppVersionNoDebug << "\">\n"
        << "  <deltas>\n"
        << "    <red>" << mdR << "</red>\n"
        << "    <green>" << mdG << "</green>\n"
        << "    <blue>" << mdB << "</blue>\n"
        << "    <alpha>" << mdA << "</alpha>\n"
        << "    <xy>" << mdXY << "</xy>\n"
        << "  </deltas>\n"
        << "  <breeder>\n"
        << "    <backgroundColor>" << colorString << "</backgroundColor>\n"
        << "    <colorMutationProbability>" << mColorMutationProbability << "</colorMutationProbability>\n"
        << "    <pointMutationProbability>" << mPointMutationProbability << "</pointMutationProbability>\n"
        << "    <pointKillProbability>" << mPointKillProbability << "</pointKillProbability>\n"
        << "    <pointEmergenceProbability>" << mPointEmergenceProbability << "</pointEmergenceProbability>\n"
        << "    <geneKillProbability>" << mGeneKillProbability << "</geneKillProbability>\n"
        << "    <geneMoveProbability>" << mGeneMoveProbability << "</geneMoveProbability>\n"
        << "    <geneEmergenceProbability>" << mGeneEmergenceProbability << "</geneEmergenceProbability>\n"
        << "    <minPointsPerGene>" << mMinPointsPerGene << "</minPointsPerGene>\n"
        << "    <maxPointsPerGene>" << mMaxPointsPerGene << "</maxPointsPerGene>\n"
        << "    <minGenes>" << mMinGenes << "</minGenes>\n"
        << "    <maxGenes>" << mMaxGenes << "</maxGenes>\n"
        << "    <minA>" << mMinA << "</minA>\n"
        << "    <maxA>" << mMaxA << "</maxA>\n"
        << "    <startDistribution>" << mStartDistribution << "</startDistribution>\n"
        << "    <scatterFactor>" << mScatterFactor << "</scatterFactor>\n"
        << "    <cores>" << mCores << "</cores>\n"
//      << "    <gpuComputing>" << mGPUComputing << "</gpuComputing>\n"
        << "  </breeder>\n"
        << "  <files>\n"
        << "    <dna>" << mCurrentDNAFile << "</dna>\n"
        << "    <image>" << mCurrentImageFile << "</image>\n"
        << "  </files>\n"
        << "  <autosave>\n"
        << "    <enabled>" << mAutoSave << "</enabled>\n"
        << "    <interval>" << mAutoSaveInterval << "</interval>\n"
        << "    <imageDirectory>" << mImageSaveDirectory << "</imageDirectory>\n"
        << "    <imageFilenameTemplate>" << mImageSaveFilenameTemplate << "</imageFilenameTemplate>\n"
        << "    <dnaDirectory>" << mDNASaveDirectory << "</dnaDirectory>\n"
        << "    <dnaFilenameTemplate>" << mDNASaveFilenameTemplate << "</dnaFilenameTemplate>\n"
        << "  </autosave>\n"
        << "</evocubist-settings>\n";
    file.close();
    return ok;
}


bool BreederSettings::load(const QString& fileName)
{
    Q_ASSERT(!fileName.isEmpty());
    QFile file(fileName);
    bool success = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!success)
        return false;
    success = read(&file);
    file.close();
    return success;
}


void BreederSettings::readBackgroundColor(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "backgroundColor");
    const QColor c(mXml.readElementText());
    mBackgroundColor = c.isValid()? c.rgba() : qRgba(255, 255, 255, 255);
}


void BreederSettings::readXY(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "xy");
    bool ok = false;
    const QString& xyString = mXml.readElementText();
    const qreal xy = xyString.toDouble(&ok);
    if (ok)
        mdXY = xy;
    else
        mXml.raiseError(QObject::tr("invalid delta xy: %1").arg(xyString));
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
        mXml.raiseError(QObject::tr("invalid delta red: %1").arg(redString));
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
        else if (mXml.name() == "red") {
            readRed();
        }
        else if (mXml.name() == "green") {
            readGreen();
        }
        else if (mXml.name() == "blue") {
            readBlue();
        }
        else if (mXml.name() == "alpha") {
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
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMinA = v;
    else
        mXml.raiseError(QObject::tr("invalid minA: %1").arg(str));
}


void BreederSettings::readMaxA(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxA");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMaxA = v;
    else
        mXml.raiseError(QObject::tr("invalid maxA: %1").arg(str));
}


void BreederSettings::readColorMutationProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "colorMutationProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mColorMutationProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid colorMutationProbability: %1").arg(str));
}


void BreederSettings::readPointMutationProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointMutationProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mPointMutationProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid pointMutationProbability: %1").arg(str));
}


void BreederSettings::readPointKillProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointKillProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mPointKillProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid pointKillProbability: %1").arg(str));
}


void BreederSettings::readPointEmergenceProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "pointEmergenceProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mPointEmergenceProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid pointEmergenceProbability: %1").arg(str));
}


void BreederSettings::readGeneKillProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneKillProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mGeneKillProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid geneKillProbability: %1").arg(str));
}


void BreederSettings::readGeneMoveProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneMoveProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mGeneMoveProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid geneMoveProbability: %1").arg(str));
}


void BreederSettings::readGeneEmergenceProbability(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "geneEmergenceProbability");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mGeneEmergenceProbability = v;
    else
        mXml.raiseError(QObject::tr("invalid geneEmergenceProbability: %1").arg(str));
}


void BreederSettings::readMinPointsPerGene(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "minPointsPerGene");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMinPointsPerGene = v;
    else
        mXml.raiseError(QObject::tr("invalid minPointsPerGene: %1").arg(str));
}


void BreederSettings::readMaxPointsPerGene(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxPointsPerGene");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMaxPointsPerGene = v;
    else
        mXml.raiseError(QObject::tr("invalid maxPointsPerGene: %1").arg(str));
}


void BreederSettings::readMinGenes(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "minGenes");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMinGenes = v;
    else
        mXml.raiseError(QObject::tr("invalid minGenes: %1").arg(str));
}


void BreederSettings::readMaxGenes(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "maxGenes");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mMaxGenes = v;
    else
        mXml.raiseError(QObject::tr("invalid maxGenes: %1").arg(str));
}


void BreederSettings::readStartDistribution(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "startDistribution");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mStartDistribution = v;
    else
        mXml.raiseError(QObject::tr("invalid startDistribution: %1").arg(str));
}


void BreederSettings::readScatterFactor(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "scatterFactor");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const qreal v = str.toDouble(&ok);
    if (ok)
        mScatterFactor = v;
    else
        mXml.raiseError(QObject::tr("invalid scatterFactor: %1").arg(str));
}


void BreederSettings::readCores(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "cores");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mCores = v;
    else
        mXml.raiseError(QObject::tr("invalid cores: %1").arg(str));
}


void BreederSettings::readGPUComputing(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "gpuComputing");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mGPUComputing = (v != 0);
    else
        mXml.raiseError(QObject::tr("invalid gpuComputing: %1").arg(str));
}


void BreederSettings::readBreeder(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "breeder");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "backgroundColor") {
            readBackgroundColor();
        }
        else if (mXml.name() == "minA") {
            readMinA();
        }
        else if (mXml.name() == "maxA") {
            readMaxA();
        }
        else if (mXml.name() == "colorMutationProbability") {
            readColorMutationProbability();
        }
        else if (mXml.name() == "pointMutationProbability") {
            readPointMutationProbability();
        }
        else if (mXml.name() == "pointKillProbability") {
            readPointKillProbability();
        }
        else if (mXml.name() == "pointEmergenceProbability") {
            readPointEmergenceProbability();
        }
        else if (mXml.name() == "geneKillProbability") {
            readGeneKillProbability();
        }
        else if (mXml.name() == "geneMoveProbability") {
            readGeneMoveProbability();
        }
        else if (mXml.name() == "geneEmergenceProbability") {
            readGeneEmergenceProbability();
        }
        else if (mXml.name() == "minPointsPerGene") {
            readMinPointsPerGene();
        }
        else if (mXml.name() == "maxPointsPerGene") {
            readMaxPointsPerGene();
        }
        else if (mXml.name() == "minGenes") {
            readMinGenes();
        }
        else if (mXml.name() == "maxGenes") {
            readMaxGenes();
        }
        else if (mXml.name() == "startDistribution") {
            readStartDistribution();
        }
        else if (mXml.name() == "scatterFactor") {
            readScatterFactor();
        }
        else if (mXml.name() == "cores") {
            readCores();
        }
        else if (mXml.name() == "gpuComputing") {
            readGPUComputing();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}


void BreederSettings::readRecentDNAFile(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "dna");
    mCurrentDNAFile = mXml.readElementText();
}


void BreederSettings::readRecentImageFile(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "image");
    mCurrentImageFile = mXml.readElementText();
}


void BreederSettings::readFiles(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "files");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "dna") {
            readRecentDNAFile();
        }
        else if (mXml.name() == "image") {
            readRecentImageFile();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}


void BreederSettings::readAutoSaveEnabled(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "enabled");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mAutoSave = (v != 0);
    else
        mXml.raiseError(QObject::tr("invalid autosave enabled value: %1").arg(str));
}


void BreederSettings::readAutoSaveInterval(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "interval");
    bool ok = false;
    const QString& str = mXml.readElementText();
    const int v = str.toInt(&ok);
    if (ok)
        mAutoSaveInterval = v;
    else
        mXml.raiseError(QObject::tr("invalid autosave interval: %1").arg(str));
}


void BreederSettings::readAutoSaveImageDirectory(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "imageDirectory");
    const QString& str = mXml.readElementText();
    if (!str.isEmpty())
        mImageSaveDirectory = str;
    else
        mXml.raiseError(QObject::tr("invalid imageDirectory: %1").arg(str));
}


void BreederSettings::readAutoSaveImageFilenameTemplate(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "imageFilenameTemplate");
    const QString& str = mXml.readElementText();
    if (!str.isEmpty())
        mImageSaveFilenameTemplate = str;
    else
        mXml.raiseError(QObject::tr("invalid imageDirectory: %1").arg(str));
}


void BreederSettings::readAutoSaveDNADirectory(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "dnaDirectory");
    const QString& str = mXml.readElementText();
    if (!str.isEmpty())
        mDNASaveDirectory = str;
    else
        mXml.raiseError(QObject::tr("invalid dnaDirectory: %1").arg(str));
}


void BreederSettings::readAutoSaveDNAFilenameTemplate(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "dnaFilenameTemplate");
    const QString& str = mXml.readElementText();
    if (!str.isEmpty())
        mDNASaveFilenameTemplate = str;
    else
        mXml.raiseError(QObject::tr("invalid dnaFilenameTemplate: %1").arg(str));
}


void BreederSettings::readAutosave(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "autosave");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "enabled") {
            readAutoSaveEnabled();
        }
        else if (mXml.name() == "interval") {
            readAutoSaveInterval();
        }
        else if (mXml.name() == "imageDirectory") {
            readAutoSaveImageDirectory();
        }
        else if (mXml.name() == "imageFilenameTemplate") {
            readAutoSaveImageFilenameTemplate();
        }
        else if (mXml.name() == "dnaDirectory") {
            readAutoSaveDNADirectory();
        }
        else if (mXml.name() == "dnaFilenameTemplate") {
            readAutoSaveDNAFilenameTemplate();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
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
    Q_ASSERT(device != NULL);
    mXml.setDevice(device);
    if (mXml.readNextStartElement()) {
        if (mXml.name() == "evocubist-settings" && mXml.attributes().value("version").toString().startsWith("1.")) {
            read();
        }
        else {
            mXml.raiseError(QObject::tr("The file is not an Evo Cubist v1.x settings file."));
        }
    }
    return !mXml.error();
}


QString BreederSettings::errorString(void) const
{
    return QObject::tr("%1 (line %2, column %3)").arg(mXml.errorString()).arg(mXml.lineNumber()).arg(mXml.columnNumber());
}

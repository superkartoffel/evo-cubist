// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BREEDERSETTINGS_H_
#define __BREEDERSETTINGS_H_

#include <QObject>
#include <QString>
#include <QRgb>
#include <QIODevice>
#include <QXmlStreamReader>


class BreederSettings : public QObject
{
    Q_OBJECT

public:
    explicit BreederSettings(void)
        : QObject(NULL)
        , mdXY(0.0042)
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
        , mGeneKillProbability(1000000)
        , mGeneMoveProbability(1000000)
        , mGeneEmergenceProbability(1000000)
        , mMinPointsPerGene(3)
        , mMaxPointsPerGene(9)
        , mMinGenes(200)
        , mMaxGenes(400)
        , mOnlyConvex(false)
        , mStartDistribution(0)
        , mScatterFactor(0.5)
        , mAutoSave(false)
        , mAutoSaveInterval(10)
        , mStopOnAutoSave(false)
        , mCores(2)
        , mBackgroundColor(qRgba(255, 255, 255, 2555))
    {
        // ...
    }

    inline QRgb backgroundColor(void) const { return mBackgroundColor; }
    inline qreal dXY(void) const { return mdXY; }
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
    inline int geneEmergenceProbability(void) const { return mGeneEmergenceProbability; }
    inline int minPointsPerGene(void) const { return mMinPointsPerGene; }
    inline int maxPointsPerGene(void) const { return mMaxPointsPerGene; }
    inline int minGenes(void) const { return mMinGenes; }
    inline int maxGenes(void) const { return mMaxGenes; }
    inline bool onlyConvex(void) const { return mOnlyConvex; }
    inline int startDistribution(void) const {  return mStartDistribution; }
    inline qreal scatterFactor(void) const { return mScatterFactor; }
    inline int cores(void) const { return mCores; }
    inline bool autoSave(void) const { return mAutoSave; }
    inline const QString& logFile(void) const { return mLogFile; }
    inline int autoSaveInterval(void) const { return mAutoSaveInterval; }
    inline bool stopOnAutoSave(void) const { return mStopOnAutoSave; }
    inline const QString& currentImageFile(void) const { return mCurrentImageFile; }
    inline const QString& currentDNAFile(void) const { return mCurrentDNAFile; }
    inline const QString& imageSaveDirectory(void) const { return mImageSaveDirectory; }
    inline const QString& imageSaveFilenameTemplate(void) const { return mImageSaveFilenameTemplate; }
    inline const QString& dnaSaveDirectory(void) const { return mDNASaveDirectory; }
    inline const QString& dnaSaveFilenameTemplate(void) const { return mDNASaveFilenameTemplate; }

    bool save(const QString& fileName);
    bool load(const QString& fileName);
    QString errorString(void) const;

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
    void setGeneEmergenceProbability(int);
    void setMinPointsPerGene(int);
    void setMaxPointsPerGene(int);
    void setMinGenes(int);
    void setMaxGenes(int);
    void setOnlyConvex(bool);
    void setCores(int);
    void setGPUComputing(bool);
    void setBackgroundColor(QRgb);
    void setAutoSaveInterval(int);
    void setAutoSave(bool);
    void setStopOnAutosave(bool);
    void setStartDistribution(int);
    void setScatterFactor(double);
    void setLogFile(const QString&);
    void setCurrentDNAFile(const QString&);
    void setCurrentImageFile(const QString&);
    void setImageSaveDirectory(const QString&);
    void setImageSaveFilenameTemplate(const QString&);
    void setDNASaveDirectory(const QString&);
    void setDNASaveFilenameTemplate(const QString&);

private:
    qreal mdXY; // [0..1)
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
    int mGeneEmergenceProbability;
    int mMinPointsPerGene; // [3..n]
    int mMaxPointsPerGene; // [n..n+x]
    int mMinGenes;
    int mMaxGenes;
    bool mOnlyConvex;
    int mStartDistribution;
    qreal mScatterFactor;
    bool mAutoSave;
    int mAutoSaveInterval; // secs
    bool mStopOnAutoSave;
    int mCores;
    bool mGPUComputing;
    QString mLogFile;
    QString mCurrentDNAFile;
    QString mCurrentImageFile;
    QString mImageSaveDirectory;
    QString mImageSaveFilenameTemplate;
    QString mDNASaveDirectory;
    QString mDNASaveFilenameTemplate;
    QRgb mBackgroundColor;

    QXmlStreamReader mXml;

    void readRecentDNAFile(void);
    void readRecentImageFile(void);
    void readLogFile(void);
    void readMinA(void);
    void readMaxA(void);
    void readColorMutationProbability(void);
    void readPointMutationProbability(void);
    void readPointKillProbability(void);
    void readPointEmergenceProbability(void);
    void readGeneKillProbability(void);
    void readGeneMoveProbability(void);
    void readGeneEmergenceProbability(void);
    void readMinPointsPerGene(void);
    void readMaxPointsPerGene(void);
    void readMinGenes(void);
    void readMaxGenes(void);
    void readStartDistribution(void);
    void readScatterFactor(void);
    void readAutoSaveEnabled(void);
    void readAutoSaveInterval(void);
    void readAutoSaveAutoStop(void);
    void readAutoSaveImageDirectory(void);
    void readAutoSaveImageFilenameTemplate(void);
    void readAutoSaveDNADirectory(void);
    void readAutoSaveDNAFilenameTemplate(void);
    void readCores(void);
    void readGPUComputing(void);
    void readBackgroundColor(void);
    void readXY(void);
    void readRed(void);
    void readGreen(void);
    void readBlue(void);
    void readAlpha(void);
    void readDeltas(void);
    void readBreeder(void);
    void readFiles(void);
    void readAutosave(void);
    void read(void);
    bool read(QIODevice*);
};


extern BreederSettings gSettings;

#endif // __BREEDERSETTINGS_H_

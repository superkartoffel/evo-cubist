// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __SVGREADER_H_
#define __SVGREADER_H_

#include <QObject>
#include <QString>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QDateTime>
#include "dna.h"

/// Read SVG file into DNA object
class SVGReader : public QObject
{
    Q_OBJECT

public:
    SVGReader(QObject* parent = NULL)
        : QObject(parent)
    { /* ... */ }
    const DNA& dna(void) const { return mDNA; }
    bool readSVG(QIODevice*);
    QString errorString(void) const;

signals:
    void deltaR(int);
    void deltaG(int);
    void deltaB(int);
    void deltaA(int);
    qreal deltaXY(qreal);

private:
    QXmlStreamReader mXml;
    DNA mDNA;

    void readSVG(void);
    void readDesc(void);
    void readTotalSeconds(void);
    void readGeneration(void);
    void readSelected(void);
    void readFitness(void);
    void readDeltaRed(void);
    void readDeltaGreen(void);
    void readDeltaBlue(void);
    void readDeltaAlpha(void);
    void readDeltaXY(void);
    void readGroup(void);
    void readPath(void);

};

#endif // __SVGREADER_H_

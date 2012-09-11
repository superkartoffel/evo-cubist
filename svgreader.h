// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __SVGREADER_H_
#define __SVGREADER_H_

#include <QString>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QDateTime>
#include "dna.h"

///
/// Read SVG file as DNA object
///
class SVGReader
{
public:
    SVGReader(void) { /* ... */ }
    const DNA& dna(void) const { return mDNA; }
    bool readSVG(QIODevice*);
    QString errorString(void) const;

    quint64 fitness(void) const { return mFitness; }
    unsigned long selected(void) const { return mSelected; }
    unsigned long generation(void) const { return mGeneration; }
    const QDateTime& dateTime(void) const { return mDate; }

private:
    QXmlStreamReader mXml;
    DNA mDNA;
    quint64 mFitness;
    unsigned long mSelected;
    unsigned long mGeneration;
    QDateTime mDate;

    void readSVG(void);
    void readDesc(void);
    void readDateTime(void);
    void readGeneration(void);
    void readSelected(void);
    void readFitness(void);
    void readGroup(void);
    void readPath(void);

};

#endif // __SVGREADER_H_

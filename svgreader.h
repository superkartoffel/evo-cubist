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

private:
    QXmlStreamReader mXml;
    DNA mDNA;

    void readSVG(void);
    void readDesc(void);
    void readTotalSeconds(void);
    void readGeneration(void);
    void readSelected(void);
    void readFitness(void);
    void readGroup(void);
    void readPath(void);

};

#endif // __SVGREADER_H_

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __DNAXMLREADER_H_
#define __DNAXMLREADER_H_

#include <QString>
#include <QSize>
#include <QXmlStreamReader>
#include "dna.h"
#include "breeder.h"

///
/// Read SVG file as DNA object
///
class DNAXmlReader
{
public:
    DNAXmlReader(void)
        : mBreeder(NULL) {}
    DNAXmlReader(Breeder* breeder)
        : mBreeder(breeder)
    {
        Q_ASSERT(breeder != NULL);
        // ...
    }
    const DNA& dna(void) const { return mDNA; }
    const QSize& size(void) const { return mSize; }
    bool readSVG(QIODevice*);
    QString DNAXmlReader::errorString(void) const;

private:
    QXmlStreamReader mXml;
    DNA mDNA;
    Breeder* mBreeder;
    QSize mSize;

    void readSVG(void);
    void readGroup(void);
    void readPath(void);

};

#endif // __DNAXMLREADER_H_

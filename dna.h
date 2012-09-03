// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __DNA_H_
#define __DNA_H_

#include <QString>
#include <QVector>
#include <QIODevice>
#include <QXmlStreamReader>
#include "genome.h"


typedef QVector<Genome> DNAType;


class DNA : public DNAType
{
public:
    enum Format { JSON, SVG };

    inline explicit DNA(void) {}
    inline ~DNA() {}
    inline DNA(const DNA &v) : DNAType(v) {}

    bool save(const QString& filename, const QSize& size, Format format = JSON) const;
    bool load(const QString& filename, Breeder* breeder, Format format = JSON);

    unsigned int points(void) const;

    QString errorString(void) const;

private:
    QSize mSize;

};

#endif // __DNA_H_

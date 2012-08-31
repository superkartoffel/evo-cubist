// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __DNA_H_
#define __DNA_H_

#include <QVector>
#include "genome.h"


typedef QVector<Genome> DNAType;


class DNA : public DNAType
{
public:
    inline DNA() {}
    inline ~DNA() {}
    inline DNA(int size);
    inline DNA(const DNA &a) : DNAType(a) {}
    inline DNA(const DNAType &v) : DNAType(v) {}

    bool save(const QString& filename) const;
    bool load(const QString& filename);

    unsigned int points(void) const;

signals:
    
public slots:

private:

};

#endif // __DNA_H_

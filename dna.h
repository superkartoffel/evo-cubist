// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __DNA_H_
#define __DNA_H_

#include <QString>
#include <QVector>
#include <QIODevice>
#include <QMutex>
#include <QMutexLocker>
#include <QXmlStreamReader>
#include "genome.h"


typedef QVector<Genome> DNAType;

class DNA
{
public:
    inline explicit DNA(void) { /* ... */ }
    inline ~DNA() { /* ... */ }
    /// deep copy constructor
    DNA(const DNA& dna);

    void mutate(void);
    bool save(const QString& filename, unsigned int generation, unsigned int selected, unsigned int fitness);
    bool load(const QString& filename);

    unsigned int points(void) const;

    void setScale(const QSize& size) { mSize = size; }

    inline void append(const Genome& genome) { mDNA.append(genome); }
    inline void clear(void) { mDNA.clear(); }
    inline int size(void) const { return mDNA.size(); }
    inline void remove(int index) { mDNA.remove(index); }
    inline void insert(int index, const Genome& genome) { mDNA.insert(index, genome); }
    inline void reserve(int size) { mDNA.reserve(size); }
    inline const Genome& at(int index) { return mDNA.at(index); }
    inline DNAType::const_iterator constBegin(void) const  { return mDNA.constBegin(); }
    inline DNAType::const_iterator constEnd(void) const { return mDNA.constEnd(); }
    inline DNAType::iterator begin(void) { return mDNA.begin(); }
    inline DNAType::iterator end(void) { return mDNA.end(); }
    inline DNAType& data(void) { return mDNA; }
    inline const DNAType& data(void) const { return mDNA; }
    inline const QSize& scale(void) const { return mSize; }

private:
    QSize mSize;
    DNAType mDNA;

    bool willMutate(unsigned int probability);
};



#endif // __DNA_H_

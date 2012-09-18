// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __DNA_H_
#define __DNA_H_

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QIODevice>
#include <QMutex>
#include <QMutexLocker>
#include <QXmlStreamReader>
#include <QtCore/QDebug>
#include <limits>
#include "gene.h"


typedef QVector<Gene> DNAType;


class DNA
{
public:
    inline explicit DNA(void)
        : mVersion(0.0)
        , mGeneration(0)
        , mSelected(0)
        , mFitness(std::numeric_limits<quint64>::max())
        , mTotalSeconds(0)
    { /* ... */ }
    inline ~DNA() { /* ... */ }
    /// deep copy constructor
    DNA(const DNA& dna);

    void mutate(void);
    bool save(QString& filename, unsigned long generation, unsigned long selected, quint64 fitness, quint64 duration);
    bool load(const QString& filename);
    const QString& errorString(void) const { return mErrorString; }

    unsigned int points(void) const;

    void setScale(const QSize& size) { mSize = size; }

    inline Gene& operator[](int index) { return mDNA[index]; }
    inline void append(const Gene& gene) { mDNA.append(gene); }
    inline void clear(void) { mDNA.clear(); }
    inline int size(void) const { return mDNA.size(); }
    inline void remove(int index) { mDNA.remove(index); }
    inline void insert(int index, const Gene& gene) { mDNA.insert(index, gene); }
    inline void reserve(int size) { mDNA.reserve(size); }
    inline const Gene& at(int index) const { return mDNA.at(index); }
    inline DNAType::const_iterator constBegin(void) const  { return mDNA.constBegin(); }
    inline DNAType::const_iterator constEnd(void) const { return mDNA.constEnd(); }
    inline DNAType::iterator begin(void) { return mDNA.begin(); }
    inline DNAType::iterator end(void) { return mDNA.end(); }
    inline const QSize& scale(void) const { return mSize; }

    inline unsigned long generation(void) const { return mGeneration; }
    inline unsigned long selected(void) const { return mSelected; }
    inline quint64 fitness(void) const { return mFitness; }
    inline qreal version(void) const { return mVersion; }
    inline quint64 totalSeconds(void) const { return mTotalSeconds; }

    inline void setGeneration(unsigned long v) { mGeneration = v; }
    inline void setSelected(unsigned long v) { mSelected = v; }
    inline void setFitness(quint64 v) { mFitness = v; }
    inline void setTotalSeconds(quint64 v) { mTotalSeconds = v; }

    QPolygonF findPolygonForPoint(const QPointF& p) const;

private:
    QSize mSize;
    DNAType mDNA;
    QString mErrorString;
    qreal mVersion;
    unsigned long mGeneration;
    unsigned long mSelected;
    quint64 mFitness;
    quint64 mTotalSeconds;

    bool willMutate(unsigned int probability);
};



#endif // __DNA_H_

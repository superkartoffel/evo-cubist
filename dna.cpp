// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QIODevice>
#include <QFile>
#include <QDataStream>
#include "dna.h"

bool DNA::save(const QString& filename) const
{
    bool rc;
    QFile file(filename);
    rc = file.open(QIODevice::WriteOnly);
    if (!rc)
        return false;
    QDataStream out(&file);
    for (DNAType::const_iterator genome = constBegin(); genome != constEnd(); ++genome)
        out << *genome;
    file.close();
    return rc;
}

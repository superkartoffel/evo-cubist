// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QtCore/QDebug>

#include "qt-json/json.h"
#include "dna.h"


using namespace QtJson;


bool DNA::save(const QString& filename) const
{
    bool rc;
    QFile file(filename);
    rc = file.open(QIODevice::WriteOnly);
    if (!rc)
        return false;
    QTextStream out(&file);
    out << "{ \"dna\": [\n";
    for (DNAType::const_iterator genome = constBegin(); genome != constEnd(); ++genome) {
        out << *genome;
        if ((genome+1) != constEnd())
            out << ",";
        out << "\n";
    }
    out << "] }\n";
    file.close();
    return rc;
}


bool DNA::load(const QString& filename)
{
    qDebug() << "DNA::load(" << filename << ")";
    bool rc;
    QFile file(filename);
    rc = file.open(QIODevice::ReadOnly);
    if (!rc)
        return false;
    QTextStream in(&file);
    QString jsonDNA = in.readAll();
    file.close();

    bool ok;
    QVariant v = Json::parse(jsonDNA, ok);
    QVariantMap result = v.toMap();
    if (ok) {
        clear();
        qDebug() << "RESULT:" << result["dna"].toString();
    }
    return rc && ok;
}

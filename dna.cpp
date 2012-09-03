// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QtCore/QDebug>


#include "qt-json/json.h"
#include "dna.h"
#include "dnaxmlreader.h"

using namespace QtJson;


// XXX: move method to Breeder
bool DNA::save(const QString& filename, const QSize& size, Format format) const
{
    bool rc;
    QFile file(filename);
    rc = file.open(QIODevice::WriteOnly);
    if (!rc)
        return false;
    QTextStream out(&file);
    switch (format) {
    case JSON:
    {
        out << "{ \"size\": { \"x\": " << size.width() << ", \"y\": " << size.height() << " },"
            << " \"dna\": [\n";
        for (DNAType::const_iterator genome = constBegin(); genome != constEnd(); ++genome) {
            out << *genome;
            if ((genome+1) != constEnd())
                out << ",";
            out << "\n";
        }
        out << "] }\n";
        break;
    }
    case SVG:
    {
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
            << " <g transform=\"scale(" << size.width() << ", " << size.height() << ")\">\n";
        for (DNAType::const_iterator genome = constBegin(); genome != constEnd(); ++genome) {
            const QColor& c = genome->color();
            out << "  <path fill=\"rgb(" << c.red() << "," << c.green() << "," << c.blue() << ")\""
                << " fill-opacity=\"" << c.alphaF() << "\""
                << " d=\"M " << genome->polygon().at(0).x() << " " << genome->polygon().at(0).x();
            for (QPolygonF::const_iterator p = genome->polygon().constBegin() + 1; p != genome->polygon().constEnd(); ++p)
                out << " L " << p->x() << " " << p->y();
            out << " Z\" />\n";
        }
        out << " </g>\n"
            << "</svg>\n";
        break;
    }
    default:
        qWarning() << "DNA::save() unknown format:" << format;
        break;
    }
    file.close();
    return rc;
}


// XXX: move method to Breeder
bool DNA::load(const QString& filename, Breeder* breeder, Format format)
{
    bool rc;
    QFile file(filename);
    rc = file.open(QIODevice::ReadOnly);
    if (!rc)
        return false;
    QTextStream in(&file);
    bool ok = false;
    switch (format) {
    case JSON:
    {
        QString jsonDNA = in.readAll();
        QVariant v = Json::parse(jsonDNA, ok);
        QVariantMap result = v.toMap();
        if (ok) {
            clear();
            qDebug() << "RESULT:" << result["dna"].toString();
        }
        break;
    }
    case SVG:
    {
        DNAXmlReader xml(breeder);
        ok = xml.readSVG(&file);
        if (ok) {
            clear();
            *this = xml.dna();
            qDebug() << "Original size: " << xml.size();
        }
        break;
    }
    default:
        qWarning() << "DNA::load() unknown format:" << format;
        ok = false;
        break;
    }
    file.close();

    return rc && ok;
}


unsigned int DNA::points(void) const
{
    unsigned int sum = 0;
    for (DNAType::const_iterator genome = constBegin(); genome != constEnd(); ++genome)
        sum += genome->size();
    return sum;
}

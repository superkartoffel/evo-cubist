// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "svgreader.h"
#include "genome.h"
#include <QRegExp>
#include <QtCore/QDebug>


void SVGReader::readPath(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "path");
    QString fill = mXml.attributes().value("fill").toString();
    // <path fill="rgb(14,9,206)" fill-opacity="0.230442" d="M 0.845225 0.845225 L 0.431106 0.585496 L 0.0788198 0.4925 L 0.0861273 0.692974 Z" />
    QRegExp fill_re("(\\d+),\\s*(\\d+),\\s*(\\d+)");
    fill_re.indexIn(fill);
    const QStringList& rgba = fill_re.capturedTexts();
    QColor color(rgba.at(1).toInt(), rgba.at(2).toInt(), rgba.at(3).toInt());
    color.setAlphaF(mXml.attributes().value("fill-opacity").toString().toDouble());

    QString d = mXml.attributes().value("d").toString();
    QRegExp coords_re("([\\d\\.]+)\\s+([\\d\\.]+)");
    int pos = 0;
    QPolygonF polygon;
    while ((pos = coords_re.indexIn(d)) >= 0) {
        const QStringList& xy = coords_re.capturedTexts();
        QPointF point(xy.at(1).toDouble(), xy.at(2).toDouble());
        polygon << point;
        d = d.right(d.size() - pos - xy.at(0).size());
    }
    mDNA.append(Genome(mBreeder, polygon, color));

    while (mXml.readNextStartElement()) {
        if (mXml.name() == "path")
            readPath();
        else
            mXml.skipCurrentElement();
    }
}


void SVGReader::readGroup(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "g");
    QString transform = mXml.attributes().value("transform").toString();
    // <g transform="scale(371, 371)">
    QRegExp re("(\\d+),\\s*(\\d+)");
    re.indexIn(transform);
    QStringList scales = re.capturedTexts();
    mSize = QSize(scales.at(1).toInt(), scales.at(2).toInt());
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "path")
            readPath();
        else
            mXml.skipCurrentElement();
    }
}


void SVGReader::readSVG(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "svg");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "g")
            readGroup();
        else
            mXml.skipCurrentElement();
    }
}


bool SVGReader::readSVG(QIODevice* device)
{
    mDNA.clear();
    mXml.setDevice(device);
    if (mXml.readNextStartElement()) {
        if (mXml.name() == "svg" && mXml.attributes().value("version") == "1.1")
            readSVG();
        else
            mXml.raiseError(QObject::tr("The file is not an SVG version 1.1 file."));
    }
    return !mXml.error();
}


QString SVGReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(mXml.errorString())
            .arg(mXml.lineNumber())
            .arg(mXml.columnNumber());
}


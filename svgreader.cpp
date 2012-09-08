// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPolygonF>
#include <QRegExp>
#include <QSize>
#include <QStringList>
#include <QtCore/QDebug>

#include "svgreader.h"
#include "genome.h"


void SVGReader::readPath(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "path");
    bool ok = false;
    QString fill = mXml.attributes().value("fill").toString();
    // <path fill="rgb(14,9,206)" fill-opacity="0.230442" d="M 0.845225 0.845225 L 0.431106 0.585496 L 0.0788198 0.4925 L 0.0861273 0.692974 Z" />
    QRegExp fill_re("(\\d+),\\s*(\\d+),\\s*(\\d+)");
    fill_re.indexIn(fill);
    const QStringList& rgba = fill_re.capturedTexts();
    const int r = rgba.at(1).toInt(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid red component in \"%1\"").arg(fill));
    const int g = rgba.at(2).toInt(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid green component in \"%1\"").arg(fill));
    const int b = rgba.at(3).toInt(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid blue component in \"%1\"").arg(fill));
    QColor color(r, g, b);
    qreal alpha = mXml.attributes().value("fill-opacity").toString().toDouble(&ok);
    if (!ok) { // try finding "fill-opacity" in "style" attribute
        // <path fill="rgb(103,81,97)" style="fill-opacity:0.215686" ... />
        const QString& style = mXml.attributes().value("style").toString();
        QRegExp fo_re("fill-opacity\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");
        fo_re.indexIn(style);
        alpha = fo_re.capturedTexts().at(1).toDouble(&ok);
        if (!ok)
            mXml.raiseError(QObject::tr("fill-opacity not found or invalid"));
    }
    color.setAlphaF(alpha);

    QPolygonF polygon;
    // ... 0.0788198 0.4925 ... 3.687e-4 -0.91112 ...
    QRegExp coords_re("([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)\\s+([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");
    QString d = mXml.attributes().value("d").toString();
    int pos = 0;
    while ((pos = coords_re.indexIn(d)) >= 0) {
        const QStringList& xy = coords_re.capturedTexts();
        const qreal x = xy.at(1).toDouble(&ok);
        if (!ok)
            mXml.raiseError(QObject::tr("invalid x coordinate in \"%1\"").arg(xy.at(0)));
        const qreal y = xy.at(3).toDouble(&ok);
        if (!ok)
            mXml.raiseError(QObject::tr("invalid y coordinate in \"%1\"").arg(xy.at(0)));
        polygon << QPointF(x, y);
        d = d.right(d.size() - pos - xy.at(0).size() + 1);
    }
    mDNA.append(Genome(polygon, color));

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
    bool ok = false;
    QString transform = mXml.attributes().value("transform").toString();
    // <g transform="scale(371, 371)">
    QRegExp re("(\\d+),\\s*(\\d+)");
    re.indexIn(transform);
    QStringList scales = re.capturedTexts();
    const int xs = scales.at(1).toInt(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid x scale factor in \"%1\"").arg(scales.at(0)));
    const int ys = scales.at(2).toInt(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid y scale factor in \"%1\"").arg(scales.at(0)));
    mDNA.setScale(QSize(xs, ys));
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

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPolygonF>
#include <QRegExp>
#include <QSize>
#include <QStringList>
#include <QtCore/QDebug>

#include "svgreader.h"
#include "genome.h"


QColor getRGB(const QRegExp& re, const QString& text, QXmlStreamReader& xml, bool* ok)
{
    if (-1 == re.indexIn(text)) {
        ok = false;
        return QColor();
    }
    const QStringList& rgba = re.capturedTexts();
    const int r = rgba.at(1).toInt(ok);
    if (!*ok)
        xml.raiseError(QObject::tr("fill: invalid red component (%2) in \"%1\"").arg(text).arg(rgba.at(1)));
    const int g = rgba.at(2).toInt(ok);
    if (!*ok)
        xml.raiseError(QObject::tr("fill: invalid green component (%2) in \"%1\"").arg(text).arg(rgba.at(2)));
    const int b = rgba.at(3).toInt(ok);
    if (!*ok)
        xml.raiseError(QObject::tr("fill: invalid blue component (%2) in \"%1\"").arg(text).arg(rgba.at(3)));
    return QColor(r, g, b);
}


void SVGReader::readPath(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "path");
    QColor color;

    bool ok = false;
    const QString& style = mXml.attributes().value("style").toString();

    // <path style="fill-opacity:0.230442;fill:rgb(14,9,206)" d="M 0.845225 0.845225 L 0.431106 0.585496 L 0.0788198 0.4925 L 0.0861273 0.692974 Z" />

    QRegExp fo_re("fill-opacity\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");
    fo_re.indexIn(style);
    qreal alpha = fo_re.capturedTexts().at(1).toDouble(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("fill-opacity (%1): not found or invalid").arg(fo_re.capturedTexts().at(1)));

    color = getRGB(QRegExp("fill\\s*:\\s*rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)"), style, mXml, &ok);
    if (!color.isValid()) { // fallback to v0.4 format
        const QString& fill = mXml.attributes().value("fill").toString();
        color = getRGB(QRegExp("(\\d+),\\s*(\\d+),\\s*(\\d+)"), fill, mXml, &ok);
    }
    if (!ok)
        mXml.raiseError(QObject::tr("fill not found or invalid"));

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


void SVGReader::readDateTime(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "datetime");
    const QString& date = mXml.readElementText();
    mDate = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss.zzz");
    if (!mDate.isValid())
        mXml.raiseError(QObject::tr("invalid date/time: %1").arg(date));
}


void SVGReader::readGeneration(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "generation");
    bool ok = false;
    const QString& gen = mXml.readElementText();
    mGeneration = gen.toULong(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid generation: %1").arg(gen));
}


void SVGReader::readSelected(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "selected");
    bool ok = false;
    const QString& sel = mXml.readElementText();
    mSelected = sel.toULong(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid selected: %1").arg(sel));
}


void SVGReader::readFitness(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "fitness");
    bool ok = false;
    const QString& fit = mXml.readElementText();
    mFitness = fit.toULongLong(&ok);
    if (!ok)
        mXml.raiseError(QObject::tr("invalid fitness: %1").arg(fit));
}


void SVGReader::readDesc(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "desc" && mXml.attributes().value("version") == "0.5");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "datetime") {
            readDateTime();
        }
        else if (mXml.name() == "generation") {
            readGeneration();
        }
        else if (mXml.name() == "selected") {
            readSelected();
        }
        else if (mXml.name() == "fitness") {
            readFitness();
        }
        else {
            mXml.skipCurrentElement();
        }
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
        if (mXml.name() == "path") {
            readPath();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}


void SVGReader::readSVG(void)
{
    Q_ASSERT(mXml.isStartElement() && mXml.name() == "svg");
    while (mXml.readNextStartElement()) {
        if (mXml.name() == "g") {
            readGroup();
        }
        else if (mXml.name() == "desc" && mXml.attributes().value("version").toString().toDouble() > 0.4) {
            readDesc();
        }
        else {
            mXml.skipCurrentElement();
        }
    }
}


bool SVGReader::readSVG(QIODevice* device)
{
    mDNA.clear();
    mXml.setDevice(device);
    if (mXml.readNextStartElement()) {
        if (mXml.name() == "svg" && mXml.attributes().value("version") == "1.1") {
            readSVG();
        }
        else {
            mXml.raiseError(QObject::tr("The file is not an SVG version 1.1 file."));
        }
    }
    return !mXml.error();
}


QString SVGReader::errorString() const
{
    return QObject::tr("%1 (line %2, column %3)").arg(mXml.errorString()).arg(mXml.lineNumber()).arg(mXml.columnNumber());
}

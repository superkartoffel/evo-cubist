// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include "helper.h"


QString secondsToTime(int seconds)
{
    const unsigned int days = seconds / 60 / 60 / 24;
    seconds -= days * 24 * 60 * 60;
    const unsigned int hours = seconds / 60 / 60;
    seconds -= hours * 60 * 60;
    const unsigned int minutes =seconds / 60;
    seconds -= minutes * 60;
    QString t = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    if (days > 0)
        t.prepend(QObject::tr("%1 %2 ").arg(days).arg(days > 1? QObject::tr("days") : QObject::tr("day")));
    return t;
}


void avoidDuplicateFilename(QString& filename)
{
    QFileInfo info(filename);
    const QString& path = info.absolutePath();
    const QString& name = info.baseName();
    const QString& suffix = info.suffix();
    for (int a = 1; QFile::exists(filename); ++a)
        filename = QString("%1/%2_%3.%4").arg(path).arg(name).arg(a, 3, 10, QChar('0')).arg(suffix);
}


bool isConvexPolygon(const QPolygonF& polygon) {
    if (polygon.isEmpty())
        return true;
    QPolygonF points = polygon;
    points.append(points.at(0));
    points.append(points.at(1));
    int oldSign = 0;
    for (int i = 0; i < points.size() - 2; ++i) {
        const QPointF& p0 = points.at(i);
        const QPointF& p1 = points.at(i+1);
        const QPointF& p2 = points.at(i+2);
        const qreal dx1 = p1.x() - p0.x();
        const qreal dy1 = p1.y() - p0.y();
        const qreal dx2 = p2.x() - p1.x();
        const qreal dy2 = p2.y() - p1.y();
        const qreal cross = dx1 * dy2 - dy1 * dx2;
        if (oldSign == 0) {
            oldSign = (cross < 0)? -1 : 1;
        }
        else {
            if (cross < 0 && oldSign > 0)
                return false;
            if (cross > 0 && oldSign < 0)
                return false;
        }
    }
    return true;
}

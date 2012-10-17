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
        t.prepend(QObject::tr("%1 %2%3").arg(days).arg(days > 1? QObject::tr("days") : QObject::tr("day")).arg(" "));
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
    static const int NO_SIGN = 0;
    int oldSign = NO_SIGN;
    for (int i = 0; i < points.size()-2; ++i) {
        const qreal c = cross(points.at(i), points.at(i+1), points.at(i+2));
        if (oldSign == NO_SIGN) {
            oldSign = (c < 0)? -1 : 1;
        }
        else {
            if (c < 0 && oldSign > 0)
                return false;
            if (c > 0 && oldSign < 0)
                return false;
        }
    }
    return true;
}


QPolygonF convexHull(QPolygonF P)
{
    const int n = P.size();
    int k = 0;
    QPolygonF H(2*n);

    // sort points lexicographically
    qSort(P.begin(), P.end(), pointLessThan);

    // build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P.at(i)) <= 0)
            --k;
        H[k++] = P.at(i);
    }
    // build upper hull
    for (int i = n-2, t = k+1; i >= 0; --i) {
        while (k >= t && cross(H[k-2], H[k-1], P.at(i)) <= 0)
            --k;
        H[k++] = P.at(i);
    }

    H.resize(k-1);
    return H;
}



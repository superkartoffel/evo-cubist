// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QObject>
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


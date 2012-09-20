// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __HELPER_H_
#define __HELPER_H_

#include <QString>
#include <QPolygonF>

extern QString secondsToTime(int);
extern void serializeFilename(QString& filename);
extern bool isConvexPolygon(const QPolygonF&);

#endif // __HELPER_H_

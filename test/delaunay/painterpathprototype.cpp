// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "painterpathprototype.h"
#include <QtScript/QScriptEngine>


Q_DECLARE_METATYPE(QPainterPath*)


PainterPathPrototype::PainterPathPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


PainterPathPrototype::~PainterPathPrototype()
{
    /* ... */
}


void PainterPathPrototype::addEllipse(qreal x, qreal y, qreal width, qreal height)
{
    thisPainterPath()->addEllipse(x, y, width, height);
}


void PainterPathPrototype::addRect(qreal x, qreal y, qreal width, qreal height)
{
    thisPainterPath()->addRect(x, y, width, height);
}


void PainterPathPrototype::moveTo(qreal x, qreal y)
{
    thisPainterPath()->moveTo(x, y);
}


void PainterPathPrototype::lineTo(qreal x, qreal y)
{
    thisPainterPath()->lineTo(x, y);
}


void PainterPathPrototype::quadTo(qreal cx, qreal cy, qreal endPointX, qreal endPointY)
{
    thisPainterPath()->quadTo(cx, cy, endPointX, endPointY);
}


void PainterPathPrototype::cubicTo(qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY)
{
    thisPainterPath()->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
}


QPainterPath* PainterPathPrototype::thisPainterPath() const
{
    return qscriptvalue_cast<QPainterPath*>(thisObject().data());
}

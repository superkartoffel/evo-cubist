// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __PAINTERPATHPROTOTYPE_H_
#define __PAINTERPATHPROTOTYPE_H_

#include <QPainterPath>
#include <QtCore/QObject>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>


class PainterPathPrototype : public QObject, public QScriptable
{
    Q_OBJECT

public:
    PainterPathPrototype(QObject* parent = NULL);
    ~PainterPathPrototype();

public slots:
    void addEllipse(qreal x, qreal y, qreal width, qreal height);
    void addRect(qreal x, qreal y, qreal width, qreal height);
    void moveTo(qreal x, qreal y);
    void lineTo(qreal x, qreal y);
    void quadTo(qreal cx, qreal cy, qreal endPointX, qreal endPointY);
    void cubicTo(qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY);

private:
    QPainterPath* thisPainterPath(void) const;
};



#endif // __PAINTERPATHPROTOTYPE_H_

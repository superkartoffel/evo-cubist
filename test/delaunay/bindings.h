// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __BINDINGS_H_
#define __BINDINGS_H_

#include <QtCore>
#include <QPointF>
#include <QPolygonF>
#include <QSharedPointer>

////////////////////////////////////////////////////////
// QPointF bindings for Qt Script
////////////////////////////////////////////////////////

typedef QSharedPointer<QPointF> QPointFPointer;
Q_DECLARE_METATYPE(QPointFPointer)

QScriptValue constructQPointF(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");
    if (context->argumentCount() != 2)
        return context->throwError(QScriptContext::SyntaxError, "QPointF needs two arguments (x, y)");
    QPointF* p = new QPointF(context->argument(0).toNumber(), context->argument(1).toNumber());
    QPointFPointer pointer(p);
    return engine->newVariant(context->thisObject(), QVariant::fromValue(pointer));
}


////////////////////////////////////////////////////////
// QPolygonF bindings for Qt Script
////////////////////////////////////////////////////////

typedef QSharedPointer<QPolygonF> QPolygonFPointer;
Q_DECLARE_METATYPE(QPolygonFPointer)

QScriptValue constructQPolygonF(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");
    QPolygonF* polygon = new QPolygonF;
    QPolygonFPointer pointer(polygon);
    return engine->newVariant(context->thisObject(), QVariant::fromValue(pointer));
}

QScriptValue qPolygonF_append(QScriptContext* context, QScriptEngine*)
{
    QPolygonFPointer polygon = qscriptvalue_cast<QPolygonFPointer>(context->thisObject());
    if (!polygon)
        return context->throwError(QScriptContext::TypeError, "this object is not a QPolygonF");
    QPointFPointer point = qscriptvalue_cast<QPointFPointer>(context->argument(0).toObject());
    polygon->append(*point);
    return context->thisObject();
}




#endif // __BINDINGS_H_

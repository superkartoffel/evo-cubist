// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "painterpathclass.h"
#include "painterpathprototype.h"



PainterPathClass::PainterPathClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QPainterPath>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new PainterPathPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


PainterPathClass::~PainterPathClass()
{
    /* ... */
}


QString PainterPathClass::name(void) const
{
    return QLatin1String("PainterPath");
}


QScriptValue PainterPathClass::prototype(void) const
{
    return mProto;
}


QScriptValue PainterPathClass::constructor(void)
{
    return mCtor;
}


QScriptValue PainterPathClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    PainterPathClass* cls = qscriptvalue_cast<PainterPathClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    QScriptValue arg = ctx->argument(0);
    if (arg.instanceOf(ctx->callee()))
        return cls->newInstance(qscriptvalue_cast<QPainterPath>(arg));
    return cls->newInstance();
}


QScriptValue PainterPathClass::toScriptValue(QScriptEngine* eng, const QPainterPath& pp)
{
    QScriptValue ctor = eng->globalObject().property("PainterPath");
    PainterPathClass* cls = qscriptvalue_cast<PainterPathClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(pp));
    return cls->newInstance(pp);
}


void PainterPathClass::fromScriptValue(const QScriptValue& obj, QPainterPath& pp)
{
    pp = qvariant_cast<QPainterPath>(obj.data().toVariant());
}


QScriptValue PainterPathClass::newInstance(void)
{
    return newInstance(QPainterPath());
}


QScriptValue PainterPathClass::newInstance(const QPainterPath &pp)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(pp));
    return engine()->newObject(this, data);
}

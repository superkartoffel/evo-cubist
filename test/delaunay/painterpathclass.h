// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __PAINTERPATHCLASS_H_
#define __PAINTERPATHCLASS_H_

#include <QPainterPath>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class PainterPathClass : public QObject, public QScriptClass
{
public:
    PainterPathClass(QScriptEngine* engine = NULL);
    ~PainterPathClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(const QPainterPath&);
    QString name(void) const;
    QScriptValue prototype(void) const;

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QPainterPath& pp);
    static void fromScriptValue(const QScriptValue& obj, QPainterPath& pp);

    QScriptValue mProto;
    QScriptValue mCtor;
};


Q_DECLARE_METATYPE(QPainterPath*)
Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(PainterPathClass*)


#endif // __PAINTERPATHCLASS_H_

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtCore/QDebug>
#include "mainwindow.h"
#include "main.h"


static const QString Company = "c't";
static const QString AppName = QObject::tr("Evo Cubist");
#ifdef QT_NO_DEBUG
static const QString AppVersion = "0.5";
#else
static const QString AppVersion = "0.5 [DEBUG]";
#endif


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    bool ok = translator.load(":/translations/evo-cubist" + QLocale::system().name());
#ifndef QT_NO_DEBUG
    if (!ok)
        qDebug() << "Could not load translations for" << QLocale::system().name() << "locale";
#endif
    if (ok)
        a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}

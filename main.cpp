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
static const QString AppUrl = "http://evo-cubist.googlecode.com/";
static const QString AppAuthor = "Oliver Lau";
static const QString AppAuthorMail = "oliver@von-und-fuer-lau.de";
static const QString AppVersionNoDebug = "0.6";
#ifdef QT_NO_DEBUG
static const QString AppVersion = AppVersionNoDebug;
#else
static const QString AppVersion = AppVersionNoDebug + " [DEBUG]";
#endif


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath("plugins");
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

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QApplication>
#include <QResource>
#include <QLocale>
#include <QTranslator>
#include <QtCore/QDebug>
#include "mainwindow.h"

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

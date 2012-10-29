// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QApplication>
#include "mainwindow.h"
#include "main.h"

const QString Company = "c't";
const QString AppName = "Voronoi";
const QString AppUrl = "http://ct.de/";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "oliver@von-und-fuer-lau.de";
const QString AppVersionNoDebug = "0.1";
const QString AppMinorVersion = ".0";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


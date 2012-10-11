// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtCore/QDebug>
#include "mainwindow.h"
#include "main.h"


const QString Company = "c't";
const QString AppName = QObject::tr("Evo Cubist");
const QString AppUrl = "http://evo-cubist.googlecode.com/";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "oliver@von-und-fuer-lau.de";
const QString AppVersionNoDebug = "1.3";
const QString AppMinorVersion = ".5";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersionNoDebug);
    a.addLibraryPath("plugins");
#ifdef Q_OS_MAC
    QCoreApplication::addLibraryPath("../plugins");
    qDebug() << QCoreApplication::libraryPaths();
#endif
    QTranslator translator;
    bool ok = translator.load(":/translations/evo-cubist_" + QLocale::system().name());
#ifndef QT_NO_DEBUG
    if (!ok)
        qWarning() << "Could not load translations for" << QLocale::system().name() << "locale";
#endif
    if (ok)
        a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}

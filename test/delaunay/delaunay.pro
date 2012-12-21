# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = delaunay
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    delaunay.cpp

HEADERS += \
    mainwindow.h \
    main.h \
    delaunay.h


FORMS += mainwindow.ui

RESOURCES +=

OTHER_FILES +=

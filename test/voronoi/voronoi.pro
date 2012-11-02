# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = voronoi
TEMPLATE = app

RESOURCES +=

OTHER_FILES +=

HEADERS += \
    ../../helper.h \
    binaryheap.h \
    point.h \
    voronoi.h \
    mainwindow.h \
    main.h \
    ../../random/rnd.h \
    ../../random/mersenne_twister.h \
    ../../random/abstract_random_number_generator.h

SOURCES += \
    voronoi.cpp \
    main.cpp \
    mainwindow.cpp \
    ../../random/rnd.cpp \
    ../../random/mersenne_twister.cpp

FORMS += \
    mainwindow.ui

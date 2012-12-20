# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui script scripttools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = delaunay
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../circle.cpp \
    ../../gene.cpp \
    ../../breedersettings.cpp \
    ../../random/rnd.cpp \
    ../../random/mersenne_twister.cpp \
    ../../helper.cpp

HEADERS += \
    mainwindow.h \
    main.h \
    ../../circle.h \
    ../../gene.h \
    ../../helper.h \
    ../../breedersettings.h \
    ../../random/rnd.h \
    ../../random/mersenne_twister.h \
    ../../random/abstract_random_number_generator.h


FORMS += mainwindow.ui

RESOURCES +=

OTHER_FILES +=

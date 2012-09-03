# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui

QMAKE_CXXFLAGS += /openmp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evo-cubist
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    imagewidget.cpp \
    generationwidget.cpp \
    breeder.cpp \
    genome.cpp \
    dna.cpp \
    qt-json/json.cpp \
    random/mersenne_twister.cpp

HEADERS += mainwindow.h \
    imagewidget.h \
    generationwidget.h \
    breeder.h \
    genome.h \
    dna.h \
    qt-json/json.h \
    random/mersenne_twister.h \
    random/abstract_random_number_generator.h

FORMS += mainwindow.ui

OTHER_FILES += \
    HINTS.txt

# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui xml

CONFIG += warn_on thread qt

# TRANSLATIONS = evo-cubist_de.ts

CODECFORTR = UTF-8



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evo-cubist
TEMPLATE = app

win32 {
QMAKE_CXXFLAGS += /openmp
RC_FILE = evo-cubist.rc
}

unix {
QMAKE_CXXFLAGS += -fopenmp
}

SOURCES += main.cpp\
    mainwindow.cpp \
    imagewidget.cpp \
    generationwidget.cpp \
    breeder.cpp \
    genome.cpp \
    dna.cpp \
    qt-json/json.cpp \
    random/mersenne_twister.cpp \
    optionsform.cpp \
    svgreader.cpp \
    breedersettings.cpp

HEADERS += mainwindow.h \
    imagewidget.h \
    generationwidget.h \
    breeder.h \
    genome.h \
    dna.h \
    qt-json/json.h \
    random/mersenne_twister.h \
    random/abstract_random_number_generator.h \
    optionsform.h \
    svgreader.h \
    breedersettings.h

FORMS += mainwindow.ui \
    optionsform.ui

OTHER_FILES += \
    HINTS.txt \
    evo-cubist.rc

RESOURCES += \
    images.qrc

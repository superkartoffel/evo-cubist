# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core gui xml svg

CONFIG += warn_on thread qt

TRANSLATIONS = evo-cubist_de.ts

CODECFORTR = UTF-8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evo-cubist
TEMPLATE = app

ICON = icons/evo-cubist.icns

RC_FILE = evo-cubist.rc

SOURCES += \
    qt-json/json.cpp \
    random/mersenne_twister.cpp \
    random/rnd.cpp \
    main.cpp \
    mainwindow.cpp \
    imagewidget.cpp \
    generationwidget.cpp \
    breeder.cpp \
    gene.cpp \
    dna.cpp \
    optionsform.cpp \
    svgreader.cpp \
    breedersettings.cpp \
    helper.cpp \
    circle.cpp \
    logviewerform.cpp \
    svgviewer.cpp

HEADERS += \
    qt-json/json.h \
    random/abstract_random_number_generator.h \
    random/mersenne_twister.h \
    random/rnd.h \
    main.h \
    mainwindow.h \
    imagewidget.h \
    generationwidget.h \
    breeder.h \
    gene.h \
    dna.h \
    optionsform.h \
    svgreader.h \
    breedersettings.h \
    individual.h \
    helper.h \
    circle.h \
    logviewerform.h \
    svgviewer.h

FORMS += mainwindow.ui \
    optionsform.ui \
    logviewerform.ui \
    svgviewer.ui

OTHER_FILES += \
    HINTS.txt \
    evo-cubist.rc \
    tools/dna2svg.py \
    TODO.txt \
    LICENSE.txt \
    README.txt \
    settings-sample.xml

RESOURCES += \
    evo-cubist.qrc

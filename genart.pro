#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T11:03:33
#
#-------------------------------------------------

QT += core gui

QMAKE_CXXFLAGS += /openmp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = genart
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    imagewidget.cpp \
    generationwidget.cpp \
    breeder.cpp \
    genome.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    generationwidget.h \
    breeder.h \
    genome.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    HINTS.txt

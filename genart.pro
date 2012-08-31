#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T11:03:33
#
#-------------------------------------------------

QT += core gui opengl

QMAKE_CXXFLAGS += /openmp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = genart
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    imagewidget.cpp \
    generationwidget.cpp \
    breeder.cpp \
    genome.cpp \
    dna.cpp \
    qt-json/json.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    generationwidget.h \
    breeder.h \
    genome.h \
    dna.h \
    qt-json/json.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    HINTS.txt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qjson-build-desktop-Qt_4_8_2__4_8_2-VS2010__Debug/lib/ -lqjson0

INCLUDEPATH += $$PWD/qjson-build-desktop-Qt_4_8_2__4_8_2-VS2010__Debug
DEPENDPATH += $$PWD/qjson-build-desktop-Qt_4_8_2__4_8_2-VS2010__Debug

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qjson-build-desktop-Qt_4_8_2__4_8_2-VS2010__Debug/lib/qjson0.lib

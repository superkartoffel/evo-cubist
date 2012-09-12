# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

QT += core testlib
QT -= gui
TARGET = evo-cubist-test
CONFIG += console qtestlib
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    ../random/mersenne_twister.cpp

HEADERS += \
    ../random/mersenne_twister.h \
    ../random/abstract_random_number_generator.h

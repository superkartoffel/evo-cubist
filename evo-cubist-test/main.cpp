// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QCoreApplication>
#include <QtCore/QDebug>
#include <QDateTime>
#include <QTest>

#include "../random/mersenne_twister.h"

class RNGTest: public QObject
 {
     Q_OBJECT

private slots:
     void initTestCase()
     {
         MT::rng.seed(QDateTime::currentDateTime().toTime_t());
     }
     void cleanupTestCase()
     {
     }

     void random1()
     {
         for (int i = 0; i < 10000; ++i) {
             qreal v = MT::random1();
             QVERIFY2(v < 1.0, "v above upper boundary");
             QVERIFY2(v >= 0.0, "v below lower boundary");
         }

     }

     void random1WithBounds()
     {
         for (int i = 0; i < 10000; ++i) {
             qreal v = MT::random1(-0.5, 0.5);
             QVERIFY2(v <= +0.5, "v above upper boundary");
             QVERIFY2(v >= -0.5, "v below lower boundary");
         }

     }

     void deviater()
     {
         for (int i = 0; i < 10000; ++i) {
             qreal v = MT::deviater(0.5, 1.0);
             QVERIFY2(v <= +1.5, "v above upper boundary");
             QVERIFY2(v >= -0.5, "v below lower boundary");
         }
     }

     void deviaterWithBounds()
     {
         for (int i = 0; i < 10000; ++i) {
             qreal v = MT::deviater(0.5, 1.0, 0.2, 0.3);
             QVERIFY2(v <= 0.7, "v above upper boundary");
             QVERIFY2(v >= 0.2, "v below lower boundary");
         }
     }
 };

#include "main.moc"

int main(int argc, char* argv[])
{
    int ok;

    RNGTest rngTest;
    ok = QTest::qExec(&rngTest, argc, argv);

    return ok;
}


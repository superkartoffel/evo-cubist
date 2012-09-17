// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QCoreApplication>
#include <QtCore/QDebug>
#include <QDateTime>
#include <QTest>

#include "../random/rnd.h"
#include "../gene.h"

class RNGTest: public QObject
{
    Q_OBJECT

private:
    static const int N = 100000;

private slots:
    void initTestCase()
    {
        qDebug() << "Seeding random number generator ...";
        rng.seed(QDateTime::currentDateTime().toTime_t());
        qDebug() << N << "iterations per test.";
    }

//    void cleanupTestCase()
//    {
//    }

    void tRandom()
    {
        for (int i = 0; i < N; ++i) {
            unsigned int v = random(256);
            QVERIFY2(v < 256, "v above upper boundary");
            QVERIFY2(v >= 0, "v below lower boundary");
        }
    }

    void tRandomWithBounds()
    {
        for (int i = 0; i < N; ++i) {
            unsigned int v = random(10, 20);
            QVERIFY2(v <= 20, "v above upper boundary");
            QVERIFY2(v >= 10, "v below lower boundary");
        }
    }

    void tDInt()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = dInt(20, 50);
            QVERIFY2(v <= 70, "v above upper boundary");
            QVERIFY2(v >= -30, "v below lower boundary");
        }

    }

    void tDIntWithBoundy()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = dInt(20, 50, 0, 30);
            QVERIFY2(v <= 30, "v above upper boundary");
            QVERIFY2(v >= 0, "v below lower boundary");
        }

    }

    void tRandom1()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = random1();
            QVERIFY2(v < 1.0, "v above upper boundary");
            QVERIFY2(v >= 0.0, "v below lower boundary");
        }
    }

    void tRandom1WithBounds()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = random1(-0.5, 0.5);
            QVERIFY2(v <= +0.5, "v above upper boundary");
            QVERIFY2(v >= -0.5, "v below lower boundary");
        }
    }

    void tdReal()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = dReal(0.5, 1.0);
            QVERIFY2(v <= +1.5, "v above upper boundary");
            QVERIFY2(v >= -0.5, "v below lower boundary");
        }
    }

    void tdRealWithBounds()
    {
        for (int i = 0; i < N; ++i) {
            qreal v = dReal(0.5, 1.0, 0.2, 0.3);
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


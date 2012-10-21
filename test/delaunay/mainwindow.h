// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPolygonF>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

#include "../../gene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);
    
private:
    Ui::MainWindow *ui;
    QPolygonF mPolygon;

    Gene mGene;
    QVector<Gene> mSplices;
    bool mShowSplices;

    QGraphicsScene mScene;

    QFuture<void> mTileThread;
    QFutureWatcher<void> mTileThreadWatcher;

    QScriptEngine mScriptEngine;
#ifndef QT_NO_DEBUG
    QScriptEngineDebugger mDebugger;
#endif
    bool mStopped;

public slots:
    void tileThreadFinished(void);
    void runStopScript(void);
    void startTiling(void);

signals:
    void tilingProgressed(int);

private: // methods
    void tileRandomly(void);
    void saveSettings(void);
    void restoreSettings(void);
};

#endif // __MAINWINDOW_H_


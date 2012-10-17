// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPolygonF>
#include <QPointF>
#include <QVector>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsScale>
#include <QGraphicsView>

#include "../../gene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    
private:
    Ui::MainWindow *ui;
    QPolygonF mPolygon;

    Gene mGene;
    QVector<Gene> mSplices;
    bool mShowSplices;

    QGraphicsScene mScene;
    QGraphicsView mView;
};

#endif // __MAINWINDOW_H_


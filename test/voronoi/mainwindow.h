// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QVector>
#include <QPaintEvent>
#include <QMouseEvent>

#include "voronoi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    
private:
    Ui::MainWindow *ui;
    Voronoi mVoronoi;
    QVector<Point> mPoints;

private: // methods
    void addPoint(int x, int y);

private slots:
    void generateItems(void);

};

#endif // __MAINWINDOW_H_

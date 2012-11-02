// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <QPainter>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../random/rnd.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RAND::initialize();
    generateItems();
    QObject::connect(ui->generatePushButton, SIGNAL(clicked()), SLOT(generateItems()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateItems(void)
{
    mVoronoi.clear();
    mPoints.clear();
    for (int i = 0; i < 100; ++i)
        addPoint(RAND::rnd(10, width()-10), RAND::rnd(10, height()-10));
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing);
    p.setBrush(Qt::transparent);
    p.setPen(Qt::darkGreen);
    for (QVector<Point>::const_iterator i = mPoints.constBegin(); i != mPoints.constEnd(); ++i) {
        p.drawEllipse(i->x(), i->y(), 2, 2);
    }
    p.setPen(Qt::darkRed);
    const QVector<Seg*>& segments = mVoronoi.segments();
    for (QVector<Seg*>::const_iterator i = segments.constBegin(); i != segments.constEnd(); ++i) {
        const Seg* const seg = *i;
        const QPointF p1(seg->start.x(), seg->start.y());
        const QPointF p2(seg->end.x(), seg->end.y());
        p.drawLine(p1, p2);
    }
}


void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
        addPoint(e->x()-1, e->y()-1);
}


void MainWindow::addPoint(int x, int y)
{
    const Point& p = Point(x, y);
    mPoints.append(p);
    QTime t0;
    t0.start();
    mVoronoi << p;
    statusBar()->showMessage(tr("%1 items (%2 ms)").arg(mPoints.size()).arg(t0.elapsed()));
    update();
}

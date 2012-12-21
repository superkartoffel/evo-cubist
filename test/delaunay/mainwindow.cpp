// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main.h"

#include <QtCore>
#include <QtCore/QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QTimer>
#include <QSettings>
#include <qmath.h>

#ifdef WIN32
#include <Windows.h>

class PerformanceTimer
{
public:
    PerformanceTimer(int& MilliSeconds)
        : mOutput(MilliSeconds)
    {
        ::QueryPerformanceCounter(&mStart);
    }
    ~PerformanceTimer(void)
    {
        LARGE_INTEGER stop;
        LARGE_INTEGER freq;
        ::QueryPerformanceCounter(&stop);
        ::QueryPerformanceFrequency(&freq);
        stop.QuadPart -= mStart.QuadPart;
        stop.QuadPart *= 1000;
        stop.QuadPart /= freq.QuadPart;
        mOutput = (stop.HighPart != 0)? -1 : stop.LowPart;
    }
protected:
    LARGE_INTEGER mStart;
    int& mOutput;
};

#endif



MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setFocus(Qt::OtherFocusReason);
    setMouseTracking(true);
    restoreSettings();
}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


void MainWindow::restoreSettings(void)
{
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
}


void MainWindow::saveSettings(void)
{
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    saveSettings();
    e->accept();
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::green);
    QPainterPath triangle;
    for (TriangleSet::const_iterator i = mTriangles.begin(); i != mTriangles.end(); ++i) {
        const Vertex* const v0 = i->vertex(0);
        const Vertex* const v1 = i->vertex(1);
        const Vertex* const v2 = i->vertex(2);
        triangle.moveTo(v0->x(), v0->y());
        triangle.lineTo(v1->x(), v1->y());
        triangle.lineTo(v2->x(), v2->y());
        triangle.lineTo(v0->x(), v0->y());
    }
    painter.drawPath(triangle);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::transparent);
    for (VertexSet::const_iterator i = mVertices.begin(); i != mVertices.end(); ++i)
        painter.drawEllipse(QPointF(i->x(), i->y()), 2.5, 2.5);
}


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    qDebug() << e->posF();
    if (mVertices.empty())
        mVertices.insert(Vertex(e->x(), e->y()));
//    else
//        *mVertices.begin() = Vertex(e->x(), e->y());
    mTriangles.clear();
    Delaunay::triangulate(mVertices, mTriangles);
    update();
}


void MainWindow::mousePressEvent(QMouseEvent* e)
{
    switch (e->button()) {
    case Qt::LeftButton:
    {
        mVertices.insert(Vertex(e->x(), e->y()));
        qDebug() << "INSERT @" << e->posF() << "size =" << mVertices.size();
        mTriangles.clear();
        int t;
        {
            PerformanceTimer timer(t);
            Delaunay::triangulate(mVertices, mTriangles);
        }
        qDebug() << t << "ms";
        break;
    }
    case Qt::RightButton:
    {
        mVertices.clear();
        mTriangles.clear();
        break;
    }
    }
    e->accept();
    update();
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
    {
        break;
    }
    default:
        break;
    }
}

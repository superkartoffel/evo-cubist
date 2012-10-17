// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../circle.h"
#include "../../random/rnd.h"
#include "../../helper.h"

#include <QPainter>
#include <QRectF>


MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , mShowSplices(false)
{
    ui->setupUi(this);
    RAND::initialize();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    painter.setBrush(Qt::green);
    painter.setPen(Qt::transparent);
    for (QPolygonF::const_iterator p = mPolygon.constBegin(); p != mPolygon.constEnd(); ++p)
        painter.drawEllipse(*p, 3.0, 3.0);
    if (mPolygon.size() == 3) {
        Circle circle(mPolygon.at(0), mPolygon.at(1), mPolygon.at(2));
        painter.setPen(QColor(255, 255, 0));
        painter.setBrush(QColor(255, 255, 0, 40));
        painter.drawEllipse(circle.center(), circle.radius(), circle.radius());
        painter.setBrush(QColor(255, 255, 0, 110));
        painter.drawEllipse(circle.center(), 1.65, 1.65);
    }
    if (isConvexPolygon(mPolygon)) {
        painter.setPen(QColor(0, 255, 0));
        painter.setBrush(QColor(0, 255, 0, 40));
    }
    else {
        painter.setPen(QColor(255, 0, 0));
        painter.setBrush(QColor(255, 0, 0, 40));
    }
    painter.drawPolygon(mPolygon);

    if (mGene.isAlive()) {
        painter.setPen(QPen(QBrush(QColor(255, 0, 0)), 3, Qt::SolidLine));
        painter.setBrush(QColor(255, 0, 0, 80));
        painter.drawPolygon(mGene.polygon());
        painter.setPen(QPen(QBrush(QColor(255, 0, 255)), 1, Qt::SolidLine));
        if (mSplices.size() > 0 && mShowSplices) {
            painter.setBrush(Qt::transparent);
            painter.setPen(QPen(QBrush(Qt::white), 1.5, Qt::DashDotLine));
            for (QVector<Gene>::const_iterator g = mSplices.constBegin(); g != mSplices.constEnd(); ++g)
                painter.drawPolygon(g->polygon());
        }
    }

    painter.setPen(QColor(255, 255, 255, 200));
    painter.drawText(QRectF(10, height()-85, 200, 80), Qt::AlignLeft | Qt::AlignBottom,
                     "GENERATED CONVEX POLYGON\r\n"
                     "S: show/hide splices\r\n"
                     "R: generate random polygon\r\n"
                     "C: clear polygon");
    painter.drawText(QRectF(width() - 210, height()-85, 200, 80), Qt::AlignRight | Qt::AlignBottom,
                     "MANUALLY PRODUCED POLYGON\r\n"
                     "left click: add point\r\n"
                     "right click: clear\r\n");
}


void MainWindow::mousePressEvent(QMouseEvent* e)
{
    switch (e->button()) {
    case Qt::LeftButton:
        mPolygon.append(e->posF());
        break;
    case Qt::RightButton:
        mPolygon.clear();
        break;
    }
    e->accept();
    update();
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_R:
    {
        QPolygonF polygon;
        const int N = RAND::rnd(3, 6);
        for (int i = 0; i < N; ++i)
            polygon << QPointF(RAND::rnd1(10, width()-10), RAND::rnd1(10, height()-10));
        polygon = convexHull(polygon);
        mGene = Gene(polygon, Qt::black);
        mSplices = mGene.splice();
        mPolygon.clear();
        update();
        break;
    }
    case Qt::Key_S:
        mShowSplices = !mShowSplices;
        update();
        break;
    case Qt::Key_C:
        mGene = Gene();
        update();
        break;
    }
}

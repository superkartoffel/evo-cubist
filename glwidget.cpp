// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent)
    : QGLWidget(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setAutoFillBackground(false);
    setStyleSheet("background-color: #333;");
}


void GLWidget::setDNA(const DNA& dna)
{
    mDNA = dna;
}


void GLWidget::setSize(const QSize& size)
{
    mSize = size;
    setMinimumSize(mSize);
}


void GLWidget::paintEvent(QPaintEvent* event)
{
    QPainter p;
    p.begin(this);
    p.setPen(Qt::transparent);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(event->rect(), QBrush(Qt::white));
    p.setRenderHint(QPainter::Antialiasing);
    p.scale(mSize.width(), mSize.height());
    for (DNAType::const_iterator genome = mDNA.constBegin(); genome != mDNA.constEnd(); ++genome) {
        p.setBrush(genome->color());
        p.drawPolygon(genome->polygon());
    }
    p.end();
}

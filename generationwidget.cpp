// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "generationwidget.h"
#include <QPainter>


GenerationWidget::GenerationWidget(QFrame* parent) :
    QFrame(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setStyleSheet("background-color: #333;");
}


void GenerationWidget::setImage(const QImage& image)
{
    mImage = image;
    update();
}


void GenerationWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(0, 0, mImage);
}

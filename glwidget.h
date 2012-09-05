// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GLWIDGET_H_
#define __GLWIDGET_H_

#include "dna.h"
#include "genome.h"

#include <QGLWidget>
#include <QSize>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget* parent = NULL);
    
    void setDNA(const DNA&);
    void setSize(const QSize&);

protected:
    void paintEvent(QPaintEvent *event);

signals:
    
public slots:
    
private:
    DNA mDNA;
    QSize mSize;
};

#endif // GLWIDGET_H

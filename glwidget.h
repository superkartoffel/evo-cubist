// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GLWIDGET_H_
#define __GLWIDGET_H_

#include "dna.h"
#include "genome.h"

#include <QImage>
#include <QWidget>
#include <QPaintEvent>
#include <QGLWidget>
#include <QSize>
#include <QGLFramebufferObject>


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget* parent = NULL);
    ~GLWidget();
    
    void setData(const DNA&, const QSize&);
    void setOriginalImage(const QImage&);

protected:
    void paintEvent(QPaintEvent *event);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


signals:
    
public slots:
    
private:
    DNA mDNA;
    QSize mSize;

    QGLFramebufferObject* render_fbo;
    QGLFramebufferObject* texture_fbo;

    QImage mOriginalImage;
};

#endif // GLWIDGET_H

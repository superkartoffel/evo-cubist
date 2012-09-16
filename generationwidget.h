// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENERATIONWIDGET_H_
#define __GENERATIONWIDGET_H_

#include <QFrame>
#include <QImage>
#include <QVector>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>

#include "dna.h"
#include "gene.h"


class GenerationWidget : public QFrame
{
    Q_OBJECT
public:
    explicit GenerationWidget(QFrame* parent = NULL);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

    const QImage& image(void) const { return mImage; }

protected:
    bool event(QEvent*);
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

signals:
    void fileDropped(const QString&);
    void clickAt(const QPointF&);
    
public slots:
    void setImage(const QImage&);
    void setDNA(const DNA&);
    void spliced(const Gene& gene, const QVector<Gene>& offsprings);


private:
    QImage mImage;
    DNA mDNA;
    QPolygonF mHighlighted;
    QRect mDestRect;
    qreal mWindowAspectRatio;
    qreal mImageAspectRatio;

    Gene mSplicedGene;
    QVector<Gene> mSplices;
};

#endif // __GENERATIONWIDGET_H_

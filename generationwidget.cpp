// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "generationwidget.h"
#include <QPainter>
#include <QUrl>
#include <QMimeData>
#include <QtCore/QDebug>
#include <qmath.h>

GenerationWidget::GenerationWidget(QFrame* parent)
    : QFrame(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setStyleSheet("background-color: #333333;");
    setFrameShape(QFrame::StyledPanel);
    setFrameStyle(QFrame::Plain);
}


void GenerationWidget::setImage(const QImage& image)
{
    mImage = image;
    setMinimumSize(mImage.size());
    update();
}


void GenerationWidget::spliced(const Gene& gene, const QVector<Gene>& offsprings)
{
    mSplicedGene = gene;
    mSplices = offsprings;
    update();
}


void GenerationWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (mImage.isNull())
        return;
    qreal windowAspectRatio = (qreal) width() / height();
    qreal imageAspectRatio = (qreal) mImage.width() / mImage.height();
    if (windowAspectRatio < imageAspectRatio) {
        const int h = int(width() / imageAspectRatio);
        mDestRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * imageAspectRatio);
        mDestRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(mDestRect, mImage);
    if (mSplicedGene.color().isValid() && !mSplices.empty()) {
        p.setRenderHint(QPainter::Antialiasing);
        p.translate(mDestRect.x(), mDestRect.y());
        p.scale(mDestRect.width(), mDestRect.height());
        p.setBrush(Qt::transparent);
        qreal invScale = 1.2 / qSqrt(mDestRect.width() * mDestRect.height());
        p.setPen(QPen(Qt::green, invScale));
        for (QVector<Gene>::const_iterator g = mSplices.constBegin(); g != mSplices.constEnd(); ++g)
            p.drawPolygon(g->polygon());
        p.setPen(QPen(Qt::red, invScale));
        p.drawPolygon(mSplicedGene.polygon());
        mSplicedGene = Gene();
        mSplices.clear();
    }
}


void GenerationWidget::mousePressEvent(QMouseEvent* e)
{
    const QPoint clickPos = e->pos() - mDestRect.topLeft();
    const QPointF p((qreal)clickPos.x() / mImage.width(), (qreal)clickPos.y() / mImage.height());
    emit clickAt(p);
}


void GenerationWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        if (d->urls().first().toString().contains(QRegExp("\\.(svg|json)$"))) {
            e->acceptProposedAction();
            setStyleSheet("background-color: #22dd22;");
        }
        else {
            e->ignore();
        }
    }
    else {
        e->ignore();
    }
}


void GenerationWidget::dragLeaveEvent(QDragLeaveEvent* e)
{
    setStyleSheet("background-color: #222222;");
    e->accept();
}


void GenerationWidget::dropEvent(QDropEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        QString fileUrl = d->urls().first().toString();
        if (fileUrl.contains(QRegExp("file://.*\\.(svg|json)$"))) {
            emit fileDropped(fileUrl.remove("file:///"));
        }
    }
    setStyleSheet("background-color: #222222;");
}

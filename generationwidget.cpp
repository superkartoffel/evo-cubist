// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPainter>
#include <QUrl>
#include <QMimeData>
#include <QtCore/QDebug>
#include <qmath.h>
#include "generationwidget.h"
#include "breedersettings.h"


GenerationWidget::GenerationWidget(QWidget* parent)
    : QWidget(parent)
    , mWindowAspectRatio(0)
    , mImageAspectRatio(0)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setMouseTracking(true);
}


void GenerationWidget::setImage(const QImage& image)
{
    mImage = image;
    mImageAspectRatio = (qreal) mImage.width() / mImage.height();
    update();
}


void GenerationWidget::setDNA(const DNA& dna)
{
    mDNA = dna;
    update();
}


void GenerationWidget::setSplices(const Gene& gene, const QVector<Gene>& offsprings)
{
    mSplicedGene = gene;
    mSplices = offsprings;
    update();
}


bool GenerationWidget::event(QEvent* e)
{
    switch (e->type()) {
    case QEvent::Leave:
        mHighlighted = QPolygonF();
        update();
        break;
    default:
        break;
    }
    return QWidget::event(e);
}


void GenerationWidget::resizeEvent(QResizeEvent* e)
{
    mWindowAspectRatio = (qreal) e->size().width() / e->size().height();
}


void GenerationWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (mImage.isNull() || qFuzzyIsNull(mImageAspectRatio) || qFuzzyIsNull(mImageAspectRatio))
        return;
    if (mWindowAspectRatio < mImageAspectRatio) {
        const int h = int(width() / mImageAspectRatio);
        mDestRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * mImageAspectRatio);
        mDestRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(mDestRect, mImage);
    const qreal invScale = 1 / qSqrt(mDestRect.width() * mDestRect.height());
    p.translate(mDestRect.x(), mDestRect.y());
    p.scale(mDestRect.width(), mDestRect.height());
    p.setBrush(Qt::transparent);
    p.setRenderHint(QPainter::Antialiasing);
    if (mSplicedGene.color().isValid() && !mSplices.empty()) {
        p.setPen(QPen(Qt::green, 1.2 * invScale));
        for (QVector<Gene>::const_iterator g = mSplices.constBegin(); g != mSplices.constEnd(); ++g)
            p.drawPolygon(g->polygon());
        p.setPen(QPen(Qt::red, 1.3 * invScale));
        p.drawPolygon(mSplicedGene.polygon());
        mSplicedGene = Gene();
        mSplices.clear();
    }
    if (!mHighlighted.isEmpty()) {
        p.setPen(QPen(QColor(255, 0, 200), 1.3 * invScale));
        p.setBrush(Qt::transparent);
        p.drawPolygon(mHighlighted);
    }
}


void GenerationWidget::mousePressEvent(QMouseEvent* e)
{
    const QPoint& clickPos = e->pos() - mDestRect.topLeft();
    emit clickAt(QPointF((qreal)clickPos.x() / mDestRect.width(), (qreal)clickPos.y() / mDestRect.height()));
}


void GenerationWidget::mouseMoveEvent(QMouseEvent* e)
{
    const QPoint& clickPos = e->pos() - mDestRect.topLeft();
    mHighlighted = mDNA.findPolygonForPoint(QPointF((qreal)clickPos.x() / mDestRect.width(), (qreal)clickPos.y() / mDestRect.height()));
    update();
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
#ifdef WIN32
            const QString fileName = fileUrl.remove("file:///");
#else
            const QString fileName = fileUrl.remove("file://");
#endif
            emit fileDropped(fileName);
            gSettings.setCurrentDNAFile(fileName);
        }
    }
    setStyleSheet("background-color: #222222;");
}

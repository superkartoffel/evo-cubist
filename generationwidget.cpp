// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "generationwidget.h"
#include <QPainter>
#include <QUrl>
#include <QMimeData>
#include <QtCore/QDebug>

GenerationWidget::GenerationWidget(QFrame* parent)
    : QFrame(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setStyleSheet("background-color: #333333;");
}


void GenerationWidget::setImage(const QImage& image)
{
    mImage = image;
    setMinimumSize(mImage.size());
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
    QRect destRect;
    if (windowAspectRatio < imageAspectRatio) {
        const int h = int(width() / imageAspectRatio);
        destRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * imageAspectRatio);
        destRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(destRect, mImage);
}


void GenerationWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        if (d->urls().first().toString().contains(QRegExp("\\.(svg|json)$")))
            e->acceptProposedAction();
    }
    setStyleSheet("background-color: #22dd22;");
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

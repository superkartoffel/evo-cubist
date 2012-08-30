// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPainter>
#include <QMimeData>
#include <QUrl>

#include "imagewidget.h"

ImageWidget::ImageWidget(QFrame* parent) :
    QFrame(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setStyleSheet("background-color: #222222;");
}


void ImageWidget::setImage(const QImage& image)
{
    mImage = image.convertToFormat(QImage::Format_ARGB32);
    update();
}


void ImageWidget::setImageFileName(const QString& name)
{
    mImageFileName = name;
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(0, 0, mImage);
}



void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        if (d->urls().first().toString().contains(QRegExp("\\.(png|jpg)$")))
            e->acceptProposedAction();
    }
    setStyleSheet("background-color: #22dd22;");
}


void ImageWidget::dragLeaveEvent(QDragLeaveEvent* e)
{
    setStyleSheet("background-color: #222222;");
    e->accept();
}


void ImageWidget::dropEvent(QDropEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        QString fileUrl = d->urls().first().toString();
        if (fileUrl.contains(QRegExp("file://.*\\.(png|jpg)$")))
            loadImage(fileUrl.remove("file:///"));
    }
    setStyleSheet("background-color: #222222;");
}


void ImageWidget::loadImage(const QString& fileName)
{
    QImage image = QImage(fileName);
    if (!image.isNull()) {
        setImage(image);
        mImageFileName = fileName;
        emit imageDropped(mImage);
    }
}

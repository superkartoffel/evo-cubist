// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPainter>
#include <QMimeData>
#include <QUrl>
#include <QtCore/QDebug>

#include "imagewidget.h"

ImageWidget::ImageWidget(QFrame* parent)
    : QFrame(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setStyleSheet("background-color: #222222;");
    setFrameShape(QFrame::StyledPanel);
    setFrameStyle(QFrame::Plain);
}


void ImageWidget::setImage(const QImage& image)
{
    mImage = image.convertToFormat(QImage::Format_ARGB32);
    setMinimumSize(mImage.size());
    update();
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    if (mImage.isNull())
        return;
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
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


void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* d = e->mimeData();
    if (d->hasUrls()) {
        if (d->urls().first().toString().contains(QRegExp("\\.(png|jpg|gif|ico|mng|tga|tiff?)$"))) {
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
        if (fileUrl.contains(QRegExp("file://.*\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
            loadImage(fileUrl.remove("file:///"));
    }
    setStyleSheet("background-color: #222222;");
}


bool ImageWidget::loadImage(const QString& fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "ImageWidget::loadImage(): fileName is empty.";
        return false;
    }
    QImage image = QImage(fileName);
    if (image.isNull())
        return false;
    setImage(image);
    mImageFileName = fileName;
    emit imageDropped(mImage);
    return true;
}

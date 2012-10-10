// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QPainter>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QtCore/QDebug>

#include "imagewidget.h"
#include "breedersettings.h"

ImageWidget::ImageWidget(QWidget* parent)
    : QWidget(parent)
    , mWindowAspectRatio(0)
    , mImageAspectRatio(0)
    , mColorPickMode(false)
    , mPipetteCursor(QPixmap(":/cursors/pipette.png"), 0, 15)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}


void ImageWidget::setImage(const QImage& image)
{
    mImage = image.convertToFormat(QImage::Format_ARGB32);
    mImageAspectRatio = (qreal)mImage.width() / mImage.height();
    update();
}


void ImageWidget::resizeEvent(QResizeEvent* e)
{
    mWindowAspectRatio = (qreal) e->size().width() / e->size().height();
}


void ImageWidget::paintEvent(QPaintEvent*)
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
}


void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls() && d->urls().first().toString().contains(QRegExp("\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
        e->acceptProposedAction();
    else
        e->ignore();
}


void ImageWidget::dragLeaveEvent(QDragLeaveEvent* e)
{
    e->accept();
}


void ImageWidget::dropEvent(QDropEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls()) {
        QString fileUrl = d->urls().first().toString();
        if (fileUrl.contains(QRegExp("file://.*\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
            loadImage(fileUrl.remove("file:///"));
    }
}


void ImageWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control) {
        mColorPickMode = true;
        setCursor(mPipetteCursor);
    }
}


void ImageWidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control) {
        mColorPickMode = false;
        setCursor(Qt::ArrowCursor);
    }
}


bool ImageWidget::cursorOnImage(const QPoint& cursorPos, /*out*/QPoint& pos) const
{
    if (mDestRect.width() == 0)
        return false;
    pos = ((qreal)mImage.width() / (qreal)mDestRect.width()) * (cursorPos - mDestRect.topLeft());
    return pos.x() >= 0 && pos.y() >= 0 && pos.x() < mImage.width() && pos.y() < mImage.height();
}


void ImageWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (mColorPickMode) {
        QPoint posInImage;
        if (cursorOnImage(e->pos(), posInImage))
            emit colorChanged(mImage.pixel(posInImage));
    }
}


void ImageWidget::mousePressEvent(QMouseEvent* e)
{
    if (mColorPickMode) {
        if (e->button() == Qt::LeftButton) {
            QPoint posInImage;
            if (cursorOnImage(e->pos(), posInImage))
                emit colorSelected(mImage.pixel(posInImage));
        }
    }
}


bool ImageWidget::loadImage(const QString& fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "ImageWidget::loadImage(): fileName is empty.";
        return false;
    }
    const QImage image(fileName);
    if (image.isNull())
        return false;
    setImage(image);
    mImageFileName = fileName;
    gSettings.setCurrentImageFile(fileName);
    emit imageDropped(mImage);
    return true;
}

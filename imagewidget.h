// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __IMAGEWIDGET_H_
#define __IMAGEWIDGET_H_

#include <QString>
#include <QRgb>
#include <QFrame>
#include <QImage>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget* parent = NULL);
    void setImage(const QImage&);
    const QString& imageFileName(void) const { return mImageFileName; }
    bool loadImage(const QString& fileName);
    const QImage& image(void) const { return mImage; }
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

signals:
    void imageDropped(const QImage&);
    void colorChanged(QRgb);
    void colorSelected(QRgb);

protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);

public slots:


private: // methods
    bool cursorOnImage(const QPoint& cursorPos, /*out*/QPoint& pos) const;

private:
    QImage mImage;
    QString mImageFileName;
    QRect mDestRect;
    qreal mWindowAspectRatio;
    qreal mImageAspectRatio;
    bool mColorPickMode;
    QCursor mPipetteCursor;
};

#endif // __IMAGEWIDGET_H_

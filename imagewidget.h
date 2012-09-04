// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __IMAGEWIDGET_H_
#define __IMAGEWIDGET_H_

#include <QString>
#include <QFrame>
#include <QImage>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

class ImageWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ImageWidget(QFrame* parent = NULL);
    void setImage(const QImage&);
    const QString& imageFileName(void) const { return mImageFileName; }
    bool loadImage(const QString& fileName);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

signals:
    void imageDropped(const QImage&);

protected:
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

public slots:


private:
    QImage mImage;
    QString mImageFileName;
};

#endif // __IMAGEWIDGET_H_

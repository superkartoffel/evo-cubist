// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENERATIONWIDGET_H_
#define __GENERATIONWIDGET_H_

#include <QFrame>
#include <QImage>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

class GenerationWidget : public QFrame
{
    Q_OBJECT
public:
    explicit GenerationWidget(QFrame* parent = NULL);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

protected:
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

signals:
    void fileDropped(const QString&);
    
public slots:
    void setImage(const QImage&);

private:
    QImage mImage;

};

#endif // __GENERATIONWIDGET_H_

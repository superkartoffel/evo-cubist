// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __GENERATIONWIDGET_H_
#define __GENERATIONWIDGET_H_

#include <QFrame>
#include <QImage>

class GenerationWidget : public QFrame
{
    Q_OBJECT
public:
    explicit GenerationWidget(QFrame* parent = NULL);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

protected:
    void paintEvent(QPaintEvent*);

signals:
    
public slots:
    void setImage(const QImage&);

private:
    QImage mImage;

};

#endif // __GENERATIONWIDGET_H_

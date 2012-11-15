// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __SVGVIEWER_H_
#define __SVGVIEWER_H_

#include <QWidget>
#include <QtSvg/QSvgWidget>
#include <QString>
#include <QResizeEvent>

namespace Ui {
class SVGViewer;
}

class SVGViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SVGViewer(QWidget* parent = NULL);
    ~SVGViewer();

    void show(const QString& filename, const QSize& sizeHint);
    QSize sizeHint(void) const { return mSizeHint; }

protected:
    void resizeEvent(QResizeEvent*);

private:
    Ui::SVGViewer *ui;
    QSvgWidget* mSvgWidget;
    qreal mAspectRatio;
    QSize mSizeHint;
};

#endif // __SVGVIEWER_H_

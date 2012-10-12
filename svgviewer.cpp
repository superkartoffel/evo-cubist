// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtCore/QDebug>
#include "main.h"
#include "svgviewer.h"
#include "ui_svgviewer.h"


SVGViewer::SVGViewer(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SVGViewer)
    , mAspectRatio(-1)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - SVG Viewer").arg(AppName));
    mSvgWidget = new QSvgWidget;
    mSvgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gridLayout->addWidget(mSvgWidget);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color: #222");
}


SVGViewer::~SVGViewer()
{
    delete ui;
    delete mSvgWidget;
}


void SVGViewer::resizeEvent(QResizeEvent* e)
{
    const qreal windowAspectRatio = (qreal)e->size().width() / e->size().height();
    if (windowAspectRatio < mAspectRatio) {
        const int h = int(width() / mAspectRatio);
        mSvgWidget->move(0, (height()-h)/2);
        mSvgWidget->resize(QSize(width(), h));
    }
    else {
        const int w = int(height() * mAspectRatio);
        mSvgWidget->move((width()-w)/2, 0);
        mSvgWidget->resize(QSize(w, height()));
    }
}


void SVGViewer::show(const QString& filename, const QSize& sizeHint)
{
    mSizeHint = sizeHint;
    mAspectRatio = (qreal)sizeHint.width() / sizeHint.height();
    setMinimumSize(120, int(120 / mAspectRatio));
    mSvgWidget->load(filename);
    activateWindow();
    QWidget::show();
}

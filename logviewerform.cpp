// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QDateTime>
#include <QString>
#include <QIcon>
#include <QtCore/QDebug>
#include "logviewerform.h"
#include "ui_logviewerform.h"


LogViewerForm::LogViewerForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogViewerForm)
{
    ui->setupUi(this);
    QObject::connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), SLOT(cellDoubleClicked(int,int)));
}


LogViewerForm::~LogViewerForm()
{
    delete ui;
}


void LogViewerForm::showEvent(QShowEvent*)
{
    raise();
}


void LogViewerForm::log(unsigned long generation, unsigned long selected, int numPoints, int numGenes, quint64 fitness, const QImage& image)
{
    const int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
    ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(QString("%1").arg(selected)));
    ui->tableWidget->setItem(rows, 2, new QTableWidgetItem(QString("%1").arg(generation)));
    ui->tableWidget->setItem(rows, 3, new QTableWidgetItem(QString("%1").arg(numPoints)));
    ui->tableWidget->setItem(rows, 4, new QTableWidgetItem(QString("%1").arg(numGenes)));
    ui->tableWidget->setItem(rows, 5, new QTableWidgetItem(QString("%1").arg(fitness)));
    ui->tableWidget->item(rows, 0)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ui->tableWidget->item(rows, 1)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->item(rows, 2)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->item(rows, 3)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->item(rows, 4)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->item(rows, 5)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    QTableWidgetItem* picItem = new QTableWidgetItem;
    picItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    picItem->setData(Qt::DecorationRole, image.scaledToHeight(32, Qt::SmoothTransformation));
    ui->tableWidget->setItem(rows, 6, picItem);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->scrollToBottom();
}


void LogViewerForm::cellDoubleClicked(int row, int column)
{
    if (column == 6) {
        // TODO: show generated image
    }
}

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QDateTime>
#include <QString>
#include "logviewerform.h"
#include "ui_logviewerform.h"


LogViewerForm::LogViewerForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogViewerForm)
{
    ui->setupUi(this);
}


LogViewerForm::~LogViewerForm()
{
    delete ui;
}


void LogViewerForm::showEvent(QShowEvent*)
{
    raise();
}


void LogViewerForm::log(unsigned long generation, unsigned long selected, int numPoints, int numGenes, quint64 fitness)
{
    const int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
    ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(QString("%1").arg(generation)));
    ui->tableWidget->setItem(rows, 2, new QTableWidgetItem(QString("%1").arg(selected)));
    ui->tableWidget->setItem(rows, 3, new QTableWidgetItem(QString("%1").arg(numPoints)));
    ui->tableWidget->setItem(rows, 4, new QTableWidgetItem(QString("%1").arg(numGenes)));
    ui->tableWidget->setItem(rows, 5, new QTableWidgetItem(QString("%1").arg(fitness)));
    ui->tableWidget->item(rows, 0)->setTextAlignment(Qt::AlignLeft  | Qt::AlignVCenter);
    ui->tableWidget->item(rows, 1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->item(rows, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->item(rows, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->item(rows, 4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->item(rows, 5)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}


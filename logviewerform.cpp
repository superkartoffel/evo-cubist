// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QDateTime>
#include <QAction>
#include <QString>
#include <QtCore/QDebug>
#include "logviewerform.h"
#include "ui_logviewerform.h"


LogViewerForm::LogViewerForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogViewerForm)
{
    ui->setupUi(this);
    mMenu = new QMenu(this);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mMenu->addAction(QIcon(":/icons/show-picture.png"), tr("Show picture"))->setData(ShowPicture);
    mMenu->addAction(QIcon(":/icons/copy-to-clipboard.png"), tr("Copy picture to clipboard"))->setData(CopyToClipboard);
    mMenu->addAction(QIcon(":/icons/go-to-folder.png"), tr("Go to folder containing picture"))->setData(GoToFolder);
    QObject::connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(provideContextMenu(const QPoint&)));
    QObject::connect(ui->clearPushButton, SIGNAL(clicked()), SLOT(clear()));
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
    const int N = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(N);
    QTableWidgetItem* picItem = new QTableWidgetItem;
    picItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    const QImage& scaledImage = image.scaledToHeight(31, Qt::SmoothTransformation);
    picItem->setData(Qt::DecorationRole, scaledImage);
    picItem->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->tableWidget->setItem(N, 0, picItem);
    ui->tableWidget->setItem(N, 1, new QTableWidgetItem(QString("%1").arg(selected)));
    ui->tableWidget->item(N, 1)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->setItem(N, 2, new QTableWidgetItem(QString("%1").arg(generation)));
    ui->tableWidget->item(N, 2)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->setItem(N, 3, new QTableWidgetItem(QString("%1").arg(numPoints)));
    ui->tableWidget->item(N, 3)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->setItem(N, 4, new QTableWidgetItem(QString("%1").arg(numGenes)));
    ui->tableWidget->item(N, 4)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->setItem(N, 5, new QTableWidgetItem(QString("%1").arg(fitness)));
    ui->tableWidget->item(N, 5)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->scrollToBottom();
}


void LogViewerForm::clear(void)
{
    int row = ui->tableWidget->rowCount();
    while (row-- > 0)
        ui->tableWidget->removeRow(row);
}


void LogViewerForm::provideContextMenu(const QPoint& p)
{
    QTableWidgetItem* item = ui->tableWidget->itemAt(p);
    if (item != NULL) {
        QAction* action = mMenu->exec(mapToGlobal(p));
        if (action != NULL) {
            bool ok = false;
            const int selected   = ui->tableWidget->item(item->row(), 1)->text().toInt(&ok);
            Q_ASSERT(ok);
            const int generation = ui->tableWidget->item(item->row(), 2)->text().toInt(&ok);
            Q_ASSERT(ok);
            switch (action->data().toInt())
            {
            case ShowPicture:
                emit showPicture(generation, selected);
                break;
            case CopyToClipboard:
                emit copyPicture(generation, selected);
                break;
            case GoToFolder:
                emit gotoPicture(generation, selected);
                break;
            default:
#ifndef QT_NO_DEBUG
                qWarning() << "unknown command:" << action->data().toInt();
#endif
                break;
            }
        }
    }
}

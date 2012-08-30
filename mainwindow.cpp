// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QDateTime>
#include <QTextStream>
#include <QtCore/QDebug>
#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"


const QString MainWindow::Company = "c't";
const QString MainWindow::AppName = QObject::tr("GenArt");
#ifdef QT_NO_DEBUG
const QString MainWindow::AppVersion = "0.1";
#else
const QString MainWindow::AppVersion = "0.1 [DEBUG]";
#endif


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName(MainWindow::Company);
    QCoreApplication::setOrganizationDomain(MainWindow::Company);
    QCoreApplication::setApplicationName(MainWindow::AppName);
    QSettings::setDefaultFormat(QSettings::NativeFormat);

    qsrand(QDateTime::currentDateTime().toTime_t());

    ui->setupUi(this);

#if _OPENMP >= 200203
    setWindowTitle(tr("%1 %2 MP").arg(MainWindow::AppName).arg(MainWindow::AppVersion));
#else
    setWindowTitle(tr("%1 %2").arg(MainWindow::AppName).arg(MainWindow::AppVersion));
#endif

    ui->horizontalLayout->removeWidget(ui->widget1);
    ui->horizontalLayout->removeWidget(ui->widget2);

    mImageWidget = new ImageWidget;
    ui->horizontalLayout->insertWidget(0, mImageWidget);
    mGenerationWidget = new GenerationWidget;
    ui->horizontalLayout->insertWidget(1, mGenerationWidget);

    QObject::connect(mImageWidget, SIGNAL(imageDropped(QImage)), &mBreeder, SLOT(setOriginalImage(QImage)));

    QObject::connect(ui->startStopPushButton, SIGNAL(clicked()), SLOT(startStop()));

    QObject::connect(&mBreeder, SIGNAL(evolved()), SLOT(evolved()), Qt::BlockingQueuedConnection);
    QObject::connect(&mBreeder, SIGNAL(proceeded()), SLOT(proceeded()), Qt::BlockingQueuedConnection);

    QObject::connect(ui->redSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaR(int)));
    QObject::connect(ui->greenSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaG(int)));
    QObject::connect(ui->blueSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaB(int)));
    QObject::connect(ui->xySlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaXY(int)));

    restoreAppSettings();
}


MainWindow::~MainWindow()
{
    delete mImageWidget;
    delete mGenerationWidget;
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    mBreeder.stop();
    mBreeder.wait();
    saveAppSettings();
    e->accept();
}


void MainWindow::evolved(void)
{
    mGenerationWidget->setImage(mBreeder.image());
    ui->fitnessLineEdit->setText(QString("%1").arg(mBreeder.currentFitness()));
    ui->selectedLineEdit->setText(QString("%1").arg(mBreeder.selected()));
    ui->polygonsLineEdit->setText(QString("%1").arg(mBreeder.dna().size()));
}


void MainWindow::proceeded(void)
{
    mGenerationWidget->setImage(mBreeder.image());
    ui->generationLineEdit->setText(QString("%1").arg(mBreeder.generation()));
}


void MainWindow::startStop(void)
{
    if (ui->startStopPushButton->text() == tr("Start")) {
        mBreeder.start(QThread::LowPriority);
        ui->startStopPushButton->setText(tr("Stop"));
    }
    else {
        mBreeder.stop();
        ui->startStopPushButton->setText(tr("Start"));
    }
}


void MainWindow::saveAppSettings(void)
{
    QSettings settings(MainWindow::Company, MainWindow::AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("MainWindow/imageFilename", mImageWidget->imageFileName());
}


void MainWindow::restoreAppSettings(void)
{
    QSettings settings(MainWindow::Company, MainWindow::AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    QString imageFileName = settings.value("MainWindow/imageFilename").toString();
    if (imageFileName != "")
        mImageWidget->loadImage(imageFileName);
}

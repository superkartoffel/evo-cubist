// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QTextStream>
#include <QtCore/QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"


const QString MainWindow::Company = "c't";
const QString MainWindow::AppName = QObject::tr("Evo Cubist");
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
    QObject::connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetBreeder()));

    QObject::connect(ui->redSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaR(int)));
    QObject::connect(ui->greenSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaG(int)));
    QObject::connect(ui->blueSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaB(int)));
    QObject::connect(ui->alphaSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaA(int)));
    QObject::connect(ui->rateSlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setMutationRate(int)));
    QObject::connect(ui->xySlider, SIGNAL(valueChanged(int)), &mBreeder, SLOT(setDeltaXY(int)));

    QObject::connect(ui->actionSaveDNA, SIGNAL(triggered()), SLOT(saveDNA()));
    QObject::connect(ui->actionOpenOriginalImage, SIGNAL(triggered()), SLOT(openOriginalImage()));
    QObject::connect(ui->actionOpenDNA, SIGNAL(triggered()), SLOT(openDNA()));

    QObject::connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    restoreAppSettings();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* e)
{
//    if (mBreeder.isRunning() || mBreeder.isDirty()) {
//        stopBreeding();
//        if (QMessageBox::question(this, tr("DNA not saved"), tr("Save DNA before quitting?")) == QMessageBox::Ok) {
//            saveDNA();
//        }
//    }
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
    ui->generationLineEdit->setText(QString("%1").arg(mBreeder.generation()));
    ui->gensSLineEdit->setText(QString("%1").arg(mBreeder.generation() / (1 + QDateTime::currentDateTime().toTime_t() - mStartTime.toTime_t())));
}


void MainWindow::startBreeding(void)
{
    mStartTime = QDateTime::currentDateTime();
    QObject::connect(&mBreeder, SIGNAL(evolved()), SLOT(evolved()), Qt::BlockingQueuedConnection);
    QObject::connect(&mBreeder, SIGNAL(proceeded()), SLOT(proceeded()), Qt::BlockingQueuedConnection);
    mBreeder.breed();
    ui->startStopPushButton->setText(tr("Stop"));
}


void MainWindow::stopBreeding(void)
{
    QObject::disconnect(&mBreeder, SIGNAL(evolved()), this, SLOT(evolved()));
    QObject::disconnect(&mBreeder, SIGNAL(proceeded()), this, SLOT(proceeded()));
    mBreeder.stop();
    mBreeder.wait();
    ui->startStopPushButton->setText(tr("Start"));
}


void MainWindow::startStop(void)
{
    if (ui->startStopPushButton->text() == tr("Start")) {
        startBreeding();
    }
    else {
        stopBreeding();
    }
}


void MainWindow::saveAppSettings(void)
{
    QSettings settings(MainWindow::Company, MainWindow::AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("MainWindow/imageFilename", mImageWidget->imageFileName());
    settings.setValue("MainWindow/deltaR", ui->redSlider->value());
    settings.setValue("MainWindow/deltaG", ui->greenSlider->value());
    settings.setValue("MainWindow/deltaB", ui->blueSlider->value());
    settings.setValue("MainWindow/deltaA", ui->alphaSlider->value());
    settings.setValue("MainWindow/deltaXY", ui->xySlider->value());
    settings.setValue("MainWindow/mutationRate", ui->rateSlider->value());
}


void MainWindow::restoreAppSettings(void)
{
    QSettings settings(MainWindow::Company, MainWindow::AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    QString imageFileName = settings.value("MainWindow/imageFilename").toString();
    if (imageFileName != "")
        mImageWidget->loadImage(imageFileName);
    ui->redSlider->setValue(settings.value("MainWindow/deltaR").toInt());
    ui->greenSlider->setValue(settings.value("MainWindow/deltaG").toInt());
    ui->blueSlider->setValue(settings.value("MainWindow/deltaB").toInt());
    ui->alphaSlider->setValue(settings.value("MainWindow/deltaA").toInt());
    ui->xySlider->setValue(settings.value("MainWindow/deltaXY").toInt());
    ui->rateSlider->setValue(settings.value("MainWindow/mutationRate").toInt());
}



void MainWindow::saveDNA(void)
{
    const QString& dnaImageFilename = QFileDialog::getSaveFileName(this, tr("DNA speichern"));
    bool success = mBreeder.dna().save(dnaImageFilename);
    if (success) {
        statusBar()->showMessage(tr("DNA unter '%1' gespeichert.").arg(dnaImageFilename), 3000);
    }
    else {
        QMessageBox::critical(this, tr("Fehler beim Speichern der DNA"), tr("Die DNA konnte nicht unter dem Namen '%1' gespeichert werden.").arg(dnaImageFilename));
    }
}


void MainWindow::loadOriginalImage(const QString& filename)
{
    if (filename != "") {
        QImage image;
        bool success = image.load(filename);
        if (success) {
            mBreeder.setOriginalImage(image);
            statusBar()->showMessage(tr("Originalbild '%1' geladen.").arg(filename), 3000);
        }
        else {
            QMessageBox::warning(this, tr("Fehler beim Laden des Originalbildes"), tr("Originalbild konnte nicht geladen werden."));
        }
    }
}


void MainWindow::openOriginalImage(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Originalbild laden"));
    loadOriginalImage(filename);
}


void MainWindow::loadDNA(const QString& filename)
{
    if (filename != "") {
        DNA dna;
        bool success = dna.load(filename);
        if (success) {
            mBreeder.setDNA(dna);
            statusBar()->showMessage(tr("DNA '%1' geladen.").arg(filename), 3000);
        }
        else {
            QMessageBox::warning(this, tr("Fehler beim Laden der DNA"), tr("DNA konnte nicht geladen werden."));
        }
    }
}


void MainWindow::openDNA(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("DNA laden"));
    loadDNA(filename);
}


void MainWindow::resetBreeder(void)
{
    QMessageBox::StandardButton button = QMessageBox::question(this, tr("Really reset breeder?"), tr("Do you really want to reset the breeder?"));
    if (button == QMessageBox::Ok) {
        stopBreeding();
        mBreeder.reset();
    }
}

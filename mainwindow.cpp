// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QHBoxLayout>
#include <QTextStream>
#include <QtCore/QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "random/mersenne_twister.h"
#include "breedersettings.h"

const QString MainWindow::Company = "c't";
const QString MainWindow::AppName = QObject::tr("Evo Cubist");
#ifdef QT_NO_DEBUG
const QString MainWindow::AppVersion = "0.2";
#else
const QString MainWindow::AppVersion = "0.2 [DEBUG]";
#endif


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName(MainWindow::Company);
    QCoreApplication::setOrganizationDomain(MainWindow::Company);
    QCoreApplication::setApplicationName(MainWindow::AppName);
    QSettings::setDefaultFormat(QSettings::NativeFormat);

    ui->setupUi(this);

#if _OPENMP >= 200203
    setWindowTitle(tr("%1 %2 MP").arg(MainWindow::AppName).arg(MainWindow::AppVersion));
#else
    setWindowTitle(tr("%1 %2").arg(MainWindow::AppName).arg(MainWindow::AppVersion));
#endif

    mImageWidget = new ImageWidget;
    QHBoxLayout* hbox1 = new QHBoxLayout;
    hbox1->addWidget(mImageWidget);
    ui->originalGroupBox->setLayout(hbox1);

    mGenerationWidget = new GenerationWidget;
    QHBoxLayout* hbox2 = new QHBoxLayout;
    hbox2->addWidget(mGenerationWidget);
    ui->generatedGroupBox->setLayout(hbox2);

    QObject::connect(mImageWidget, SIGNAL(imageDropped(QImage)), &mBreeder, SLOT(setOriginalImage(QImage)));
    QObject::connect(mGenerationWidget, SIGNAL(fileDropped(QString)), SLOT(loadDNA(QString)));

    QObject::connect(&mAutoSaveTimer, SIGNAL(timeout()), SLOT(autoSaveGeneratedImage()));

    QObject::connect(ui->startStopPushButton, SIGNAL(clicked()), SLOT(startStop()));
    QObject::connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetBreeder()));

    QObject::connect(ui->redSlider, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setDeltaR(int)));
    QObject::connect(ui->greenSlider, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setDeltaG(int)));
    QObject::connect(ui->blueSlider, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setDeltaB(int)));
    QObject::connect(ui->alphaSlider, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setDeltaA(int)));
    QObject::connect(ui->xySlider, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setDeltaXY(int)));

    QObject::connect(ui->actionSaveDNA, SIGNAL(triggered()), SLOT(saveDNA()));
    QObject::connect(ui->actionOpenDNA, SIGNAL(triggered()), SLOT(openDNA()));
    QObject::connect(ui->actionOpenOriginalImage, SIGNAL(triggered()), SLOT(openOriginalImage()));

    QObject::connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));
    QObject::connect(ui->actionOptions, SIGNAL(triggered()), &mOptionsForm, SLOT(show()));

    MT::rng.seed(QDateTime::currentDateTime().toTime_t());

    restoreAppSettings();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    saveAppSettings();
    if (mBreeder.isRunning()) {
        stopBreeding();
    }
    if (mBreeder.isDirty()) {
        QMessageBox msgBox;
        msgBox.setText(tr("<b>DNA has been modified.</b>"));
        msgBox.setInformativeText(tr("You have unsaved DNA. Do you want to save it?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            saveDNA();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            e->ignore();
            return;
        }
    }
    e->accept();
}


void MainWindow::proceeded(unsigned int generation)
{
    ui->generationLineEdit->setText(QString("%1").arg(generation));
    ui->gensSLineEdit->setText(QString("%1").arg((qreal)generation / (1 + QDateTime::currentDateTime().toTime_t() - mStartTime.toTime_t())));
}


void MainWindow::evolved(const QImage& image, const DNA& dna, unsigned int fitness, unsigned int selected, unsigned generation)
{
    mGenerationWidget->setImage(image);
    ui->fitnessLineEdit->setText(QString("%1").arg(fitness));
    ui->selectedLineEdit->setText(QString("%1").arg(selected));
    ui->selectedRatioLineEdit->setText(QString("%1%").arg(1e2 * selected / generation));
    ui->polygonsLineEdit->setText(QString("%1").arg(dna.size()));
    ui->pointsLineEdit->setText(QString("%1").arg(dna.points()));
}


void MainWindow::autoSaveGeneratedImage(void)
{
    const QString& imageFilename = mOptionsForm.imageFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    mGenerationWidget->image().save(imageFilename);
    const QString& dnaFilename = mOptionsForm.dnaFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    const DNA& dna = mBreeder.dna();
    mBreeder.dnaMutex()->lock();
    dna.save(dnaFilename, mBreeder.originalImage().size());
    mBreeder.dnaMutex()->unlock();
    statusBar()->showMessage(tr("Automatically saved '%1' and '%2'.").arg(imageFilename).arg(dnaFilename), 1000);
}


void MainWindow::startBreeding(void)
{
    statusBar()->showMessage(tr("Starting ..."), 3000);
    ui->startStopPushButton->setText(tr("Stop"));
    mStartTime = QDateTime::currentDateTime();
    QObject::connect(&mBreeder, SIGNAL(evolved(QImage, DNA, unsigned int, unsigned int, unsigned int)), SLOT(evolved(QImage, DNA, unsigned int, unsigned int, unsigned int)), Qt::BlockingQueuedConnection);
    QObject::connect(&mBreeder, SIGNAL(proceeded(unsigned int)), this, SLOT(proceeded(unsigned int)), Qt::BlockingQueuedConnection);
    mBreeder.breed();
    if (mOptionsForm.autoSave()) {
        mAutoSaveTimer.setInterval(1000 * mOptionsForm.saveInterval());
        mAutoSaveTimer.start();
    }
}


void MainWindow::stopBreeding(void)
{
    QObject::disconnect(&mBreeder, SIGNAL(evolved(QImage, DNA, unsigned int, unsigned int, unsigned int)), this, SLOT(evolved(QImage, DNA, unsigned int, unsigned int, unsigned int)));
    QObject::disconnect(&mBreeder, SIGNAL(proceeded(unsigned int)), this, SLOT(proceeded(unsigned int)));
    ui->startStopPushButton->setText(tr("Start"));
    mAutoSaveTimer.stop();
    mBreeder.stop();
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
    settings.setValue("MainWindow/lastSavedDNA", mLastSavedDNA);
    settings.setValue("MainWindow/lastSavedSVG", mLastSavedSVG);
    settings.setValue("Options/geometry", mOptionsForm.saveGeometry());
    settings.setValue("Options/deltaR", ui->redSlider->value());
    settings.setValue("Options/deltaG", ui->greenSlider->value());
    settings.setValue("Options/deltaB", ui->blueSlider->value());
    settings.setValue("Options/deltaA", ui->alphaSlider->value());
    settings.setValue("Options/deltaXY", ui->xySlider->value());
    settings.setValue("Options/colorMutationRate", mOptionsForm.colorMutationRate());
    settings.setValue("Options/pointMutationRate", mOptionsForm.pointMutationRate());
    settings.setValue("Options/pointKillRate", mOptionsForm.pointKillRate());
    settings.setValue("Options/pointEmergenceRate", mOptionsForm.pointEmergenceRate());
    settings.setValue("Options/genomeKillRate", mOptionsForm.genomeKillRate());
    settings.setValue("Options/genomeEmergenceRate", mOptionsForm.genomeEmergenceRate());
    settings.setValue("Options/imageSaveDirectory", mOptionsForm.imageSaveDirectory());
    settings.setValue("Options/imageSaveFilenameTemplate", mOptionsForm.imageSaveFilenameTemplate());
    settings.setValue("Options/dnaSaveDirectory", mOptionsForm.dnaSaveDirectory());
    settings.setValue("Options/dnaSaveFilenameTemplate", mOptionsForm.dnaSaveFilenameTemplate());
    settings.setValue("Options/saveInterval", mOptionsForm.saveInterval());
    settings.setValue("Options/autoSave", mOptionsForm.autoSave());
}


void MainWindow::restoreAppSettings(void)
{
    QSettings settings(MainWindow::Company, MainWindow::AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    QString imageFileName = settings.value("MainWindow/imageFilename").toString();
    if (imageFileName != "")
        mImageWidget->loadImage(imageFileName);
    ui->redSlider->setValue(settings.value("Options/deltaR", 100).toInt());
    ui->greenSlider->setValue(settings.value("Options/deltaG", 100).toInt());
    ui->blueSlider->setValue(settings.value("Options/deltaB", 100).toInt());
    ui->alphaSlider->setValue(settings.value("Options/deltaA", 100).toInt());
    ui->xySlider->setValue(settings.value("Options/deltaXY", 230).toInt());
    mOptionsForm.restoreGeometry(settings.value("Options/geometry").toByteArray());
    mOptionsForm.setImageSaveDirectory(settings.value("Options/imageSaveDirectory", QDir::homePath()).toString());
    mOptionsForm.setImageSaveFilenameTemplate(settings.value("Options/imageSaveFilenameTemplate", "%1-%2-%3.png").toString());
    mOptionsForm.setDNASaveDirectory(settings.value("Options/dnaSaveDirectory", QDir::homePath()).toString());
    mOptionsForm.setDNASaveFilenameTemplate(settings.value("Options/dnaSaveFilenameTemplate", "%1-%2-%3.svg").toString());
    mOptionsForm.setSaveInterval(settings.value("Options/saveInterval", 10).toInt());
    mOptionsForm.setAutoSave(settings.value("Options/autoSave", true).toBool());
    mOptionsForm.setColorMutationRate(settings.value("Options/colorMutationRate", 700).toInt());
    mOptionsForm.setPointMutationRate(settings.value("Options/pointMutationRate", 700).toInt());
    mOptionsForm.setPointKillRate(settings.value("Options/pointKillRate", 700).toInt());
    mOptionsForm.setPointEmergenceRate(settings.value("Options/pointEmergenceRate", 700).toInt());
    mOptionsForm.setGenomeKillRate(settings.value("Options/genomeKillRate", 700).toInt());
    mOptionsForm.setGenomeEmergenceRate(settings.value("Options/genomeEmergenceRate", 700).toInt());
}


void MainWindow::saveDNA(void)
{
    const QString& dnaFilename = QFileDialog::getSaveFileName(this, tr("Save DNA"), QString(), tr("DNA files (*.svg; *.json; *.dna)"));
    if (dnaFilename.isNull())
        return;
    bool success = mBreeder.dna().save(dnaFilename, mBreeder.originalImage().size());
    if (success) {
        statusBar()->showMessage(tr("DNA saved as '%1'.").arg(dnaFilename), 5000);
        mLastSavedDNA = dnaFilename;
    }
    else {
        QMessageBox::warning(this, tr("Error saving DNA"), tr("JSON-formatted DNA could not be saved as '%1'.").arg(dnaFilename));
    }
}


void MainWindow::openOriginalImage(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load original picture"));
    loadOriginalImage(filename);
}


void MainWindow::loadOriginalImage(const QString& filename)
{
    if (filename != "") {
        bool success = mImageWidget->loadImage(filename);
        if (success) {
            statusBar()->showMessage(tr("Original picture '%1' loaded.").arg(filename), 3000);
        }
        else {
            QMessageBox::warning(this, tr("Error loading the original picture."), tr("Original picture could not be loaded."));
        }
    }
}


void MainWindow::loadDNA(const QString& filename)
{
    if (filename != "") {
        DNA dna;
        bool success = dna.load(filename);
        if (success) {
            stopBreeding();
            mBreeder.setDNA(dna);
            statusBar()->showMessage(tr("DNA '%1' loaded.").arg(filename), 3000);
        }
        else {
            QMessageBox::warning(this, tr("Error loading DNA"), tr("JSON-formatted DNA could not be loaded."));
        }
    }
}


void MainWindow::openDNA(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load DNA"), QString(), tr("DNA files (*.svg; *.json; *.dna)"));
    loadDNA(filename);
}


void MainWindow::resetBreeder(void)
{
    if (QMessageBox::question(this, tr("Really reset breeder?"), tr("Do you really want to reset the breeder?")) == QMessageBox::Ok) {
        stopBreeding();
        mBreeder.reset();
    }
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About Evo Cubist"),
                       tr("<p><b>Evo Cubist</b> calculates vector images from bitmaps by using genetic algorithms. "
                          "See <a href=\"http://evo-cubist.googlecode.com/\">http://evo-cubist.googlecode.com/</a> for more info.</p>"
                          "<p>Copyright &copy; 2012 Oliver Lau &lt;oliver@von-und-fuer.lau.de&gt;</p>"
                          "<p>Licensed under the Apache License, Version 2.0 (the \"License\"); "
                          "you may not use this file except in compliance with the License. "
                          "You may obtain a copy of the License at</p>"
                          "<p style=\"margin: 3ex 0 3ex 3em;\"><a href=\"http://www.apache.org/licenses/LICENSE-2.0\">http://www.apache.org/licenses/LICENSE-2.0</p>"
                          "Unless required by applicable law or agreed to in writing, software "
                          "distributed under the License is distributed on an \"AS IS\" BASIS, "
                          "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. "
                          "See the License for the specific language governing permissions and "
                          "limitations under the License."));
}


void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this);
}

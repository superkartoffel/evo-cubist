// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QHBoxLayout>
#include <QTextStream>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QThread>
#include <QRegExp>
#include <QStringList>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "random/mersenne_twister.h"
#include "breedersettings.h"
#include "main.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mPriority(QThread::InheritPriority)
{
    QCoreApplication::setOrganizationName(Company);
    QCoreApplication::setOrganizationDomain(Company);
    QCoreApplication::setApplicationName(AppName);
    QSettings::setDefaultFormat(QSettings::NativeFormat);

    ui->setupUi(this);
    setWindowTitle(tr("%1 %2").arg(AppName).arg(AppVersion));

    mImageWidget = new ImageWidget;
    QHBoxLayout* hbox1 = new QHBoxLayout;
    hbox1->addWidget(mImageWidget);
    ui->originalGroupBox->setLayout(hbox1);

    mGenerationWidget = new GenerationWidget;
    QHBoxLayout* hbox2 = new QHBoxLayout;
    hbox2->addWidget(mGenerationWidget);
    ui->generatedGroupBox->setLayout(hbox2);

    QObject::connect(mImageWidget, SIGNAL(imageDropped(QImage)), &mBreeder, SLOT(setOriginalImage(QImage)));
    QObject::connect(mImageWidget, SIGNAL(imageDropped(QImage)), SLOT(imageDropped(QImage)));
    QObject::connect(mGenerationWidget, SIGNAL(fileDropped(QString)), SLOT(loadDNA(QString)));

    QObject::connect(&mAutoSaveTimer, SIGNAL(timeout()), SLOT(autoSaveGeneratedImage()));

    QObject::connect(&mOptionsForm, SIGNAL(priorityChanged(QThread::Priority)), SLOT(priorityChanged(QThread::Priority)));
    QObject::connect(&mOptionsForm, SIGNAL(autoSaveIntervalChanged(int)), SLOT(autoSaveIntervalChanged(int)));
    QObject::connect(&mOptionsForm, SIGNAL(autoSaveToggled(bool)), SLOT(autoSaveToggled(bool)));

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
    QObject::connect(ui->actionOptions, SIGNAL(triggered()), &mOptionsForm, SLOT(raise()));

    MT::rng.seed(QDateTime::currentDateTime().toTime_t());

    restoreAppSettings();

    proceeded(1);
    evolved(mBreeder.image(), mBreeder.dna(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());

    QObject::connect(&mBreeder, SIGNAL(finished()), SLOT(breederFinished()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    stopBreeding();
    saveAppSettings();
    mOptionsForm.close();
    if (!mBreeder.isDirty())
        return;

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
    case QMessageBox::Cancel:
        e->ignore();
        return;
    case QMessageBox::Discard:
        break;
    }
    e->accept();
}


void MainWindow::priorityChanged(QThread::Priority priority)
{
    mPriority = priority;
    bool wasRunning = mBreeder.isRunning();
    mBreeder.stop();
    if (wasRunning)
        mBreeder.breed(mPriority);
}


void MainWindow::proceeded(unsigned long generation)
{
    ui->generationLineEdit->setText(QString("%1").arg(generation));
    ui->gensSLineEdit->setText(QString("%1").arg((qreal)generation / (1 + QDateTime::currentDateTime().toTime_t() - mStartTime.toTime_t())));
}


void MainWindow::evolved(const QImage& image, const DNA& dna, quint64 fitness, unsigned long selected, unsigned long generation)
{
    const int numPoints = dna.points();
    mGenerationWidget->setImage(image);
    ui->fitnessLineEdit->setText(QString("%1").arg(fitness));
    ui->selectedLineEdit->setText(QString("%1").arg(selected));
    ui->selectedRatioLineEdit->setText(QString("%1%").arg(1e2 * selected / generation));
    ui->polygonsLineEdit->setText(QString("%1").arg(dna.size()));
    ui->pointsLineEdit->setText(QString("%1").arg(numPoints));
    if (mLog.isOpen()) {
        QTextStream log(&mLog);
        // (Zeitstempel, Generation, Selected, Points, Genomes, Fitness)
        log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " " <<
               generation << " " <<
               selected << " " <<
               numPoints << " " <<
               dna.size() << " " <<
               fitness << "\n";
    }
}


void MainWindow::autoSaveGeneratedImage(void)
{
    const QCursor oldCursor = cursor();
    setCursor(Qt::WaitCursor);
    const QString& imageFilename = mOptionsForm.imageFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    mGenerationWidget->image().save(imageFilename);
    const QString& dnaFilename = mOptionsForm.dnaFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    DNA dna = mBreeder.dna(); // gives a clone
    bool success = dna.save(dnaFilename, mBreeder.generation(), mBreeder.selected(), mBreeder.currentFitness());
    if (success)
        statusBar()->showMessage(tr("Automatically saved mutation %1 out of %2 generations.").arg(mBreeder.selected()).arg(mBreeder.generation()), 3000);
    else
        statusBar()->showMessage(tr("Automatic saving failed."), 3000);
    setCursor(oldCursor);
}


void MainWindow::autoSaveIntervalChanged(int interval)
{
    if (mOptionsForm.autoSave()) {
        mAutoSaveTimer.stop();
        mAutoSaveTimer.setInterval(1000 * interval);
        mAutoSaveTimer.start();
    }
}


void MainWindow::autoSaveToggled(bool enabled)
{
    if (enabled) {
        mAutoSaveTimer.setInterval(1000 * mOptionsForm.saveInterval());
        mAutoSaveTimer.start();
    }
    else {
        mAutoSaveTimer.stop();
    }
}


void MainWindow::startBreeding(void)
{
    QFileInfo info;
    // check if DNA save directory exists
    info.setFile(mOptionsForm.dnaSaveDirectory());
    if (!info.exists() || !info.isWritable() || !info.isDir()) {
        QMessageBox::warning(this, tr("DNA save directory missing"), tr("The selected DNA save directory does not exist. Please go to the options dialog and choose a new one. Then try starting again."));
        mOptionsForm.go("Autosave", "dnaSaveDirectory");
        return;
    }
    // check if image save directory exists
    info.setFile(mOptionsForm.imageSaveDirectory());
    if (!info.exists() || !info.isWritable() || !info.isDir()) {
        QMessageBox::warning(this, tr("Image save directory missing"), tr("The selected image save directory does not exist. Please go to the options dialog and choose a new one. Then try starting again."));
        mOptionsForm.go("Autosave", "imageSaveDirectory");
        return;
    }
    // check if log file is writable
    info.setFile(mOptionsForm.logFile());
    QFileInfo dirInfo(info.absolutePath());
    if (!dirInfo.isDir() || !dirInfo.isWritable()) {
        QMessageBox::warning(this, tr("Log file is not writable"), tr("The selected log file is not writable. Please go to the options dialog and choose a new one. Then try starting again."));
        mOptionsForm.go("Autosave", "logFile");
        return;
    }

    statusBar()->showMessage(tr("Starting ..."), 3000);
    if (!mOptionsForm.logFile().isEmpty()) {
        mLog.setFileName(mOptionsForm.logFile());
        mLog.open(QIODevice::Append | QIODevice::Text);
        if (mLog.isOpen()) {
            QTextStream log(&mLog);
            log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " " << "START.\n";
        }
    }
    mStartTime = QDateTime::currentDateTime();
    QObject::connect(&mBreeder,
                     SIGNAL(evolved(QImage, DNA, quint64, unsigned long, unsigned long)),
                     this,
                     SLOT(evolved(QImage, DNA, quint64, unsigned long, unsigned long)),
                     Qt::BlockingQueuedConnection);
    QObject::connect(&mBreeder,
                     SIGNAL(proceeded(unsigned long)),
                     this,
                     SLOT(proceeded(unsigned long)),
                     Qt::BlockingQueuedConnection);
    mBreeder.breed(mPriority);
    if (mOptionsForm.autoSave()) {
        mAutoSaveTimer.setInterval(1000 * mOptionsForm.saveInterval());
        mAutoSaveTimer.start();
        if (ui->startStopPushButton->text() == tr("Start"))
            autoSaveGeneratedImage();
    }
    ui->startStopPushButton->setText(tr("Pause"));
}


void MainWindow::stopBreeding(void)
{
    if (!mBreeder.isRunning())
        return;
    ui->startStopPushButton->setText(tr("Resume"));
    mAutoSaveTimer.stop();
    mBreeder.stop();
    QObject::disconnect(&mBreeder,
                        SIGNAL(evolved(QImage, DNA, quint64, unsigned long, unsigned long)),
                        this,
                        SLOT(evolved(QImage, DNA, quint64, unsigned long, unsigned long)));
    QObject::disconnect(&mBreeder,
                        SIGNAL(proceeded(unsigned long)),
                        this,
                        SLOT(proceeded(unsigned long)));
    if (mLog.isOpen()) {
        QTextStream log(&mLog);
        log << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " " << "STOP.\n";
        mLog.close();
    }
}


void MainWindow::startStop(void)
{
    if (ui->startStopPushButton->text() == tr("Start") || ui->startStopPushButton->text() == tr("Resume")) {
        startBreeding();
    }
    else {
        stopBreeding();
    }
}


void MainWindow::saveAppSettings(void)
{
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("MainWindow/imageFilename", mImageWidget->imageFileName());
    settings.setValue("MainWindow/lastSavedDNA", mLastSavedDNA);
    settings.setValue("Options/geometry", mOptionsForm.saveGeometry());
    settings.setValue("Options/deltaR", ui->redSlider->value());
    settings.setValue("Options/deltaG", ui->greenSlider->value());
    settings.setValue("Options/deltaB", ui->blueSlider->value());
    settings.setValue("Options/deltaA", ui->alphaSlider->value());
    settings.setValue("Options/deltaXY", ui->xySlider->value());
    settings.setValue("Options/colorMutationProbability", mOptionsForm.colorMutationProbability());
    settings.setValue("Options/pointMutationProbability", mOptionsForm.pointMutationProbability());
    settings.setValue("Options/pointKillProbability", mOptionsForm.pointKillProbability());
    settings.setValue("Options/pointEmergenceProbability", mOptionsForm.pointEmergenceProbability());
    settings.setValue("Options/genomeKillProbability", mOptionsForm.genomeKillProbability());
    settings.setValue("Options/genomeMoveProbability", mOptionsForm.genomeMoveProbability());
    settings.setValue("Options/genomeEmergenceProbability", mOptionsForm.genomeEmergenceProbability());
    settings.setValue("Options/minPointsPerGenome", mOptionsForm.minPointsPerGenome());
    settings.setValue("Options/maxPointsPerGenome", mOptionsForm.maxPointsPerGenome());
    settings.setValue("Options/minGenomes", mOptionsForm.minGenomes());
    settings.setValue("Options/maxGenomes", mOptionsForm.maxGenomes());
    settings.setValue("Options/minAlpha", mOptionsForm.minAlpha());
    settings.setValue("Options/maxAlpha", mOptionsForm.maxAlpha());
    settings.setValue("Options/imageSaveDirectory", mOptionsForm.imageSaveDirectory());
    settings.setValue("Options/imageSaveFilenameTemplate", mOptionsForm.imageSaveFilenameTemplate());
    settings.setValue("Options/dnaSaveDirectory", mOptionsForm.dnaSaveDirectory());
    settings.setValue("Options/dnaSaveFilenameTemplate", mOptionsForm.dnaSaveFilenameTemplate());
    settings.setValue("Options/logFile", mOptionsForm.logFile());
    settings.setValue("Options/saveInterval", mOptionsForm.saveInterval());
    settings.setValue("Options/autoSave", mOptionsForm.autoSave());
    settings.setValue("Options/startDistribution", mOptionsForm.startDistribution());
    settings.setValue("Options/scatterFactor", mOptionsForm.scatterFactor());
    settings.setValue("Options/cores", mOptionsForm.cores());
}


void MainWindow::restoreAppSettings(void)
{
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    QString imageFileName = settings.value("MainWindow/imageFilename", ":/images/Mona-Lisa-256x256.png").toString();
    mImageWidget->loadImage(imageFileName);
    mLastSavedDNA = settings.value("MainWindow/lastSavedDNA").toString();
    ui->redSlider->setValue(settings.value("Options/deltaR", 50).toInt());
    ui->greenSlider->setValue(settings.value("Options/deltaG", 50).toInt());
    ui->blueSlider->setValue(settings.value("Options/deltaB", 50).toInt());
    ui->alphaSlider->setValue(settings.value("Options/deltaA", 50).toInt());
    ui->xySlider->setValue(settings.value("Options/deltaXY", 230).toInt());
    mOptionsForm.restoreGeometry(settings.value("Options/geometry").toByteArray());
    mOptionsForm.setImageSaveDirectory(settings.value("Options/imageSaveDirectory", QDir::homePath()).toString());
    mOptionsForm.setImageSaveFilenameTemplate(settings.value("Options/imageSaveFilenameTemplate", "%1-%2-%3.png").toString());
    mOptionsForm.setDNASaveDirectory(settings.value("Options/dnaSaveDirectory", QDir::homePath()).toString());
    mOptionsForm.setDNASaveFilenameTemplate(settings.value("Options/dnaSaveFilenameTemplate", "%1-%2-%3.svg").toString());
    mOptionsForm.setSaveInterval(settings.value("Options/saveInterval", 10).toInt());
    mOptionsForm.setAutoSave(settings.value("Options/autoSave", true).toBool());
    mOptionsForm.setLogFile(settings.value("Options/logFile").toString());
    mOptionsForm.setCores(settings.value("Options/cores", QThread::idealThreadCount()).toInt());
    mOptionsForm.setStartDistribution(settings.value("Options/startDistribution", 4).toInt());
    mOptionsForm.setScatterFactor(settings.value("Options/scatterFactor", 0.5).toDouble());
    mOptionsForm.setColorMutationProbability(settings.value("Options/colorMutationProbability", 1000).toInt());
    mOptionsForm.setPointMutationProbability(settings.value("Options/pointMutationProbability", 1000).toInt());
    mOptionsForm.setPointKillProbability(settings.value("Options/pointKillProbability", 1000).toInt());
    mOptionsForm.setPointEmergenceProbability(settings.value("Options/pointEmergenceProbability", 1000).toInt());
    mOptionsForm.setGenomeKillProbability(settings.value("Options/genomeKillProbability", 1000).toInt());
    mOptionsForm.setGenomeMoveProbability(settings.value("Options/genomeMoveProbability", 5000).toInt());
    mOptionsForm.setGenomeEmergenceProbability(settings.value("Options/genomeEmergenceProbability", 1000).toInt());
    mOptionsForm.setMinPointsPerGenome(settings.value("Options/minPointsPerGenome", 3).toInt());
    mOptionsForm.setMaxPointsPerGenome(settings.value("Options/maxPointsPerGenome", 5).toInt());
    mOptionsForm.setMinGenomes(settings.value("Options/minGenomes", 150).toInt());
    mOptionsForm.setMaxGenomes(settings.value("Options/maxGenomes", 500).toInt());
    mOptionsForm.setMinAlpha(settings.value("Options/minAlpha", 5).toInt());
    mOptionsForm.setMaxAlpha(settings.value("Options/maxAlpha", 45).toInt());
    mBreeder.reset();
}


void MainWindow::saveDNA(void)
{
    const QString& dnaFilename = QFileDialog::getSaveFileName(this, tr("Save DNA"), QString(), tr("DNA files (*.svg; *.json; *.dna)"));
    if (dnaFilename.isNull())
        return;
    DNA dna = mBreeder.dna();
    bool success = dna.save(dnaFilename, mBreeder.generation(), mBreeder.selected(), mBreeder.currentFitness());
    if (success) {
        statusBar()->showMessage(tr("DNA saved as '%1'.").arg(dnaFilename), 5000);
        mLastSavedDNA = dnaFilename;
    }
    else {
        QMessageBox::warning(this, tr("Error saving DNA"), tr("DNA could not be saved as '%1'.").arg(dnaFilename));
    }
    mBreeder.setDirty(false);
}


void MainWindow::imageDropped(const QImage& image)
{
    Q_UNUSED(image);
    evolved(mBreeder.image(), mBreeder.dna(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
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
            QRegExp re(".*(\\d{10}).*(\\d{9})");
            re.indexIn(filename);
            const QStringList& n = re.capturedTexts();
            mBreeder.setGeneration(n.at(1).toULong());
            mBreeder.setSelected(n.at(2).toULong());
            mBreeder.setDNA(dna);
            proceeded(mBreeder.generation());
            evolved(mBreeder.image(), mBreeder.dna(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
            statusBar()->showMessage(tr("DNA '%1' loaded.").arg(filename), 3000);
        }
        else {
            QMessageBox::warning(this, tr("Error loading DNA"), tr("DNA could not be loaded. Reason: %1").arg(dna.errorString()));
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
    bool ok = !mBreeder.isDirty();
    if (!ok)
        ok = QMessageBox::question(this, tr("Really reset breeder?"), tr("Do you really want to reset the breeder?")) == QMessageBox::Ok;
    if (ok) {
        stopBreeding();
        mBreeder.reset();
        evolved(mBreeder.image(), mBreeder.dna(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
        ui->startStopPushButton->setText(tr("Start"));
    }
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About %1 %2").arg(AppName).arg(AppVersion),
                       tr("<p><b>%1</b> calculates vector images from bitmaps by using genetic algorithms. "
                          "See <a href=\"%1\">%1</a> for more info.</p>"
                          "<p>Copyright &copy; 2012 Oliver Lau &lt;oliver@von-und-fuer.lau.de&gt;</p>"
                          "<p>Licensed under the Apache License, Version 2.0 (the \"License\"); "
                          "you may not use this file except in compliance with the License. "
                          "You may obtain a copy of the License at</p>"
                          "<p style=\"margin: 3ex 0 3ex 3em;\"><a href=\"http://www.apache.org/licenses/LICENSE-2.0\">http://www.apache.org/licenses/LICENSE-2.0</p>"
                          "Unless required by applicable law or agreed to in writing, software "
                          "distributed under the License is distributed on an \"AS IS\" BASIS, "
                          "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. "
                          "See the License for the specific language governing permissions and "
                          "limitations under the License.").arg(AppName).arg(AppUrl).arg(AppUrl));
}


void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this);
}

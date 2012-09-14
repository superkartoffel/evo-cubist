// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QtGlobal>
#include <QHBoxLayout>
#include <QDockWidget>
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
#include "random/rnd.h"
#include "breedersettings.h"
#include "main.h"
#include "helper.h"
#include <limits>

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

    mOptionsForm = new OptionsForm;

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
    QObject::connect(mGenerationWidget, SIGNAL(clickAt(const QPointF&)), &mBreeder, SLOT(spliceAt(const QPointF&)), Qt::DirectConnection);
    QObject::connect(&mBreeder, SIGNAL(spliced(Gene,QVector<Gene>)), mGenerationWidget, SLOT(spliced(Gene,QVector<Gene>)));

    QObject::connect(&mAutoSaveTimer, SIGNAL(timeout()), SLOT(autoSaveGeneratedImage()));

    QObject::connect(mOptionsForm, SIGNAL(priorityChanged(QThread::Priority)), SLOT(priorityChanged(QThread::Priority)));
    QObject::connect(mOptionsForm, SIGNAL(autoSaveIntervalChanged(int)), SLOT(autoSaveIntervalChanged(int)));
    QObject::connect(mOptionsForm, SIGNAL(autoSaveToggled(bool)), SLOT(autoSaveToggled(bool)));

    QObject::connect(ui->startStopPushButton, SIGNAL(clicked()), SLOT(startStop()));
    QObject::connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetBreeder()));

    QObject::connect(ui->redSlider, SIGNAL(valueChanged(int)), &gSettings, SLOT(setDeltaR(int)));
    QObject::connect(ui->greenSlider, SIGNAL(valueChanged(int)), &gSettings, SLOT(setDeltaG(int)));
    QObject::connect(ui->blueSlider, SIGNAL(valueChanged(int)), &gSettings, SLOT(setDeltaB(int)));
    QObject::connect(ui->alphaSlider, SIGNAL(valueChanged(int)), &gSettings, SLOT(setDeltaA(int)));
    QObject::connect(ui->xySlider, SIGNAL(valueChanged(int)), &gSettings, SLOT(setDeltaXY(int)));
    QObject::connect(ui->redSlider, SIGNAL(valueChanged(int)), SLOT(setDeltaR(int)));
    QObject::connect(ui->greenSlider, SIGNAL(valueChanged(int)), SLOT(setDeltaG(int)));
    QObject::connect(ui->blueSlider, SIGNAL(valueChanged(int)), SLOT(setDeltaB(int)));
    QObject::connect(ui->alphaSlider, SIGNAL(valueChanged(int)), SLOT(setDeltaA(int)));
    QObject::connect(ui->xySlider, SIGNAL(valueChanged(int)), SLOT(setDeltaXY(int)));

    QObject::connect(ui->actionSaveDNA, SIGNAL(triggered()), SLOT(saveDNA()));
    QObject::connect(ui->actionOpenDNA, SIGNAL(triggered()), SLOT(openDNA()));
    QObject::connect(ui->actionOpenOriginalImage, SIGNAL(triggered()), SLOT(openOriginalImage()));

    QObject::connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));
    QObject::connect(ui->actionOptions, SIGNAL(triggered()), mOptionsForm, SLOT(show()));
    QObject::connect(ui->actionOptions, SIGNAL(triggered()), mOptionsForm, SLOT(raise()));

    rng.seed(QDateTime::currentDateTime().toTime_t());

    restoreAppSettings();

    mStartTime = QDateTime::currentDateTime();
    proceeded(1);
    evolved(mBreeder.image(), mBreeder.dna(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
}


MainWindow::~MainWindow()
{
    delete mOptionsForm;
    delete mImageWidget;
    delete mGenerationWidget;
    delete ui;
}


bool MainWindow::event(QEvent* e)
{
    return QMainWindow::event(e);
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    stopBreeding();
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
        case QMessageBox::Cancel:
            e->ignore();
            return;
        case QMessageBox::Discard:
            break;
        }
    }
    saveAppSettings();
    mOptionsForm->close();
    e->accept();
}


void MainWindow::setDeltaR(int v)
{
    ui->rLineEdit->setText(QString("%1").arg(v));
}


void MainWindow::setDeltaG(int v)
{
    ui->gLineEdit->setText(QString("%1").arg(v));
}


void MainWindow::setDeltaB(int v)
{
    ui->bLineEdit->setText(QString("%1").arg(v));
}


void MainWindow::setDeltaA(int v)
{
    ui->aLineEdit->setText(QString("%1").arg(v));
}


void MainWindow::setDeltaXY(int v)
{
    ui->xyLineEdit->setText(QString("%1").arg(1e-4*(qreal)v));
}


void MainWindow::priorityChanged(QThread::Priority priority)
{
    mPriority = priority;
    bool wasRunning = mBreeder.isRunning();
    mBreeder.stop();
    if (wasRunning)
        mBreeder.breed(mPriority);
}


quint64 MainWindow::totalSeconds(void) const {
    quint64 totalseconds = mBreeder.totalSeconds() + QDateTime::currentDateTime().toTime_t() - mStartTime.toTime_t();
    if (totalseconds == 0)
        totalseconds = 1;
    return totalseconds;
}


void MainWindow::proceeded(unsigned long generation)
{
    quint64 totalseconds = totalSeconds();
    ui->gensSLineEdit->setText(QString("%1").arg((qreal)generation / totalseconds));
    ui->generationLineEdit->setText(QString("%1").arg(generation));
    ui->totalTimeLineEdit->setText(secondsToTime(totalseconds));
}


void MainWindow::evolved(const QImage& image, const DNA& dna, quint64 fitness, unsigned long selected, unsigned long generation)
{
    const int numPoints = dna.points();
    mGenerationWidget->setImage(image);
    if (fitness == std::numeric_limits<quint64>::max())
        ui->fitnessLineEdit->setText(tr("n/a"));
    else
        ui->fitnessLineEdit->setText(QString("%1").arg(fitness));
    ui->selectedLineEdit->setText(QString("%1").arg(selected));
    ui->selectedRatioLineEdit->setText(QString("%1%").arg(1e2 * selected / generation));
    ui->polygonsLineEdit->setText(QString("%1").arg(dna.size()));
    ui->pointsLineEdit->setText(QString("%1").arg(numPoints));
    if (mLog.isOpen()) {
        QTextStream log(&mLog);
        // (Zeitstempel, Generation, Selected, Points, Genes, Fitness)
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
    const QString& imageFilename = mOptionsForm->imageFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    mGenerationWidget->image().save(imageFilename);
    const QString& dnaFilename = mOptionsForm->dnaFilename(mImageWidget->imageFileName(), mBreeder.generation(), mBreeder.selected());
    DNA dna = mBreeder.dna(); // gives a clone
    bool success = dna.save(dnaFilename, mBreeder.generation(), mBreeder.selected(), mBreeder.currentFitness(), totalSeconds());
    if (success)
        statusBar()->showMessage(tr("Automatically saved mutation %1 out of %2 generations.").arg(mBreeder.selected()).arg(mBreeder.generation()), 3000);
    else
        statusBar()->showMessage(tr("Automatic saving failed."), 3000);
    setCursor(oldCursor);
}


void MainWindow::autoSaveIntervalChanged(int interval)
{
    if (mOptionsForm->autoSave()) {
        bool wasActive = mAutoSaveTimer.isActive();
        mAutoSaveTimer.stop();
        mAutoSaveTimer.setInterval(1000 * interval);
        if (wasActive)
            mAutoSaveTimer.start();
    }
}


void MainWindow::autoSaveToggled(bool enabled)
{
    if (enabled) {
        bool wasActive = mAutoSaveTimer.isActive();
        mAutoSaveTimer.setInterval(1000 * mOptionsForm->saveInterval());
        if (wasActive)
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
    info.setFile(mOptionsForm->dnaSaveDirectory());
    if (!info.exists() || !info.isWritable() || !info.isDir()) {
        QMessageBox::warning(this, tr("DNA save directory missing"), tr("The selected DNA save directory does not exist. Please go to the options dialog and choose a new one. Then try starting again."));
        mOptionsForm->go("Autosave", "dnaSaveDirectory");
        return;
    }
    // check if image save directory exists
    info.setFile(mOptionsForm->imageSaveDirectory());
    if (!info.exists() || !info.isWritable() || !info.isDir()) {
        QMessageBox::warning(this, tr("Image save directory missing"), tr("The selected image save directory does not exist. Please go to the options dialog and choose a new one. Then try starting again."));
        mOptionsForm->go("Autosave", "imageSaveDirectory");
        return;
    }
    // check if log file is writable
    if (!mOptionsForm->logFile().isEmpty()) {
        info.setFile(mOptionsForm->logFile());
        QFileInfo dirInfo(info.absolutePath());
        if (!dirInfo.isDir() || !dirInfo.isWritable()) {
            QMessageBox::warning(this, tr("Log file is not writable"), tr("The selected log file is not writable. Please go to the options dialog and choose a new one. Then try starting again."));
            mOptionsForm->go("Autosave", "logFile");
            return;
        }
    }

    statusBar()->showMessage(tr("Starting ..."), 3000);
    if (!mOptionsForm->logFile().isEmpty()) {
        mLog.setFileName(mOptionsForm->logFile());
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
    if (mOptionsForm->autoSave()) {
        mAutoSaveTimer.setInterval(1000 * mOptionsForm->saveInterval());
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
    mBreeder.addTotalSeconds(QDateTime::currentDateTime().toTime_t() - mStartTime.toTime_t());
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
    settings.setValue("Options/geometry", mOptionsForm->saveGeometry());
    settings.setValue("Options/deltaR", ui->redSlider->value());
    settings.setValue("Options/deltaG", ui->greenSlider->value());
    settings.setValue("Options/deltaB", ui->blueSlider->value());
    settings.setValue("Options/deltaA", ui->alphaSlider->value());
    settings.setValue("Options/deltaXY", ui->xySlider->value());
    settings.setValue("Options/colorMutationProbability", mOptionsForm->colorMutationProbability());
    settings.setValue("Options/pointMutationProbability", mOptionsForm->pointMutationProbability());
    settings.setValue("Options/pointKillProbability", mOptionsForm->pointKillProbability());
    settings.setValue("Options/pointEmergenceProbability", mOptionsForm->pointEmergenceProbability());
    settings.setValue("Options/geneKillProbability", mOptionsForm->geneKillProbability());
    settings.setValue("Options/geneMoveProbability", mOptionsForm->geneMoveProbability());
    settings.setValue("Options/geneEmergenceProbability", mOptionsForm->geneEmergenceProbability());
    settings.setValue("Options/minPointsPerGene", mOptionsForm->minPointsPerGene());
    settings.setValue("Options/maxPointsPerGene", mOptionsForm->maxPointsPerGene());
    settings.setValue("Options/minGenes", mOptionsForm->minGenes());
    settings.setValue("Options/maxGenes", mOptionsForm->maxGenes());
    settings.setValue("Options/minAlpha", mOptionsForm->minAlpha());
    settings.setValue("Options/maxAlpha", mOptionsForm->maxAlpha());
    settings.setValue("Options/imageSaveDirectory", mOptionsForm->imageSaveDirectory());
    settings.setValue("Options/imageSaveFilenameTemplate", mOptionsForm->imageSaveFilenameTemplate());
    settings.setValue("Options/dnaSaveDirectory", mOptionsForm->dnaSaveDirectory());
    settings.setValue("Options/dnaSaveFilenameTemplate", mOptionsForm->dnaSaveFilenameTemplate());
    settings.setValue("Options/logFile", mOptionsForm->logFile());
    settings.setValue("Options/saveInterval", mOptionsForm->saveInterval());
    settings.setValue("Options/autoSave", mOptionsForm->autoSave());
    settings.setValue("Options/startDistribution", mOptionsForm->startDistribution());
    settings.setValue("Options/scatterFactor", mOptionsForm->scatterFactor());
    settings.setValue("Options/cores", mOptionsForm->cores());
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
    ui->xySlider->setValue(settings.value("Options/deltaXY", 2300).toInt());
    mOptionsForm->restoreGeometry(settings.value("Options/geometry").toByteArray());
    mOptionsForm->setImageSaveDirectory(settings.value("Options/imageSaveDirectory", QDir::homePath()).toString());
    mOptionsForm->setImageSaveFilenameTemplate(settings.value("Options/imageSaveFilenameTemplate", "%1-%2-%3.png").toString());
    mOptionsForm->setDNASaveDirectory(settings.value("Options/dnaSaveDirectory", QDir::homePath()).toString());
    mOptionsForm->setDNASaveFilenameTemplate(settings.value("Options/dnaSaveFilenameTemplate", "%1-%2-%3.svg").toString());
    mOptionsForm->setSaveInterval(settings.value("Options/saveInterval", 10).toInt());
    mOptionsForm->setAutoSave(settings.value("Options/autoSave", true).toBool());
    mOptionsForm->setLogFile(settings.value("Options/logFile").toString());
    mOptionsForm->setCores(settings.value("Options/cores", QThread::idealThreadCount()).toInt());
    mOptionsForm->setStartDistribution(settings.value("Options/startDistribution", 4).toInt());
    mOptionsForm->setScatterFactor(settings.value("Options/scatterFactor", 0.45).toDouble());
    mOptionsForm->setColorMutationProbability(settings.value("Options/colorMutationProbability", 1000).toInt());
    mOptionsForm->setPointMutationProbability(settings.value("Options/pointMutationProbability", 1000).toInt());
    mOptionsForm->setPointKillProbability(settings.value("Options/pointKillProbability", 1000).toInt());
    mOptionsForm->setPointEmergenceProbability(settings.value("Options/pointEmergenceProbability", 1000).toInt());
    mOptionsForm->setGeneKillProbability(settings.value("Options/geneKillProbability", 1000).toInt());
    mOptionsForm->setGeneMoveProbability(settings.value("Options/geneMoveProbability", 5000).toInt());
    mOptionsForm->setGeneEmergenceProbability(settings.value("Options/geneEmergenceProbability", 1000).toInt());
    mOptionsForm->setMinPointsPerGene(settings.value("Options/minPointsPerGene", 3).toInt());
    mOptionsForm->setMaxPointsPerGene(settings.value("Options/maxPointsPerGene", 5).toInt());
    mOptionsForm->setMinGenes(settings.value("Options/minGenes", 150).toInt());
    mOptionsForm->setMaxGenes(settings.value("Options/maxGenes", 500).toInt());
    mOptionsForm->setMinAlpha(settings.value("Options/minAlpha", 5).toInt());
    mOptionsForm->setMaxAlpha(settings.value("Options/maxAlpha", 45).toInt());
    mBreeder.reset();
}


void MainWindow::saveDNA(void)
{
    const QString& dnaFilename = QFileDialog::getSaveFileName(this, tr("Save DNA"), QString(), tr("DNA files (*.svg; *.json; *.dna)"));
    if (dnaFilename.isNull())
        return;
    DNA dna = mBreeder.dna();
    bool success = dna.save(dnaFilename, mBreeder.generation(), mBreeder.selected(), mBreeder.currentFitness(), totalSeconds());
    if (success) {
        statusBar()->showMessage(tr("DNA saved as '%1'.").arg(dnaFilename), 5000);
        mLastSavedDNA = dnaFilename;
    }
    else {
        QMessageBox::warning(this, tr("Error saving DNA"), tr("DNA could not be saved as '%1'.").arg(dnaFilename));
    }
    mBreeder.setDirty(false);
}


void MainWindow::imageDropped(const QImage&)
{
    evolved(mBreeder.image(), mBreeder.constDNA(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
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
            if (dna.generation() == 0 || dna.selected() == 0) { // evaluation of metadata section failed
                QRegExp re("(\\d{10}).+(\\d{9})");
                const int pos = re.indexIn(filename);
                if (-1 != pos) {
                    const QStringList& n = re.capturedTexts();
                    mBreeder.setGeneration(n.at(1).toULong());
                    mBreeder.setSelected(n.at(2).toULong());
                }
                else {
                    qWarning() << "MainWindow::loadDNA(" << filename << ") Cannot determine generation and selected from filename or metadata. Tried metadata first.";
                }
            }
            mStartTime = QDateTime::currentDateTime();
            proceeded(mBreeder.generation());
            evolved(mBreeder.image(), mBreeder.constDNA(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
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
        mStartTime = QDateTime::currentDateTime();
        mBreeder.reset();
        proceeded(1);
        evolved(mBreeder.image(), mBreeder.constDNA(), mBreeder.currentFitness(), mBreeder.selected(), mBreeder.generation());
        ui->startStopPushButton->setText(tr("Start"));
    }
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About %1 %2").arg(AppName).arg(AppVersionNoDebug),
                       tr("<p><b>%1</b> calculates vector images from bitmaps by using genetic algorithms. "
                          "See <a href=\"%2\" title=\"%1 project homepage\">%2</a> for more info.</p>"
                          "<p>Copyright &copy; 2012 %3 &lt;%4&gt;</p>"
                          "<p>This program is free software: you can redistribute it and/or modify "
                          "it under the terms of the GNU General Public License as published by "
                          "the Free Software Foundation, either version 3 of the License, or "
                          "(at your option) any later version.</p>"
                          "<p>This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                          "GNU General Public License for more details.</p>"
                          "You should have received a copy of the GNU General Public License "
                          "along with this program. "
                          "If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0\">http://www.gnu.org/licenses</a></p>.")
                       .arg(AppName).arg(AppUrl).arg(AppAuthor).arg(AppAuthorMail));
}


void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this);
}

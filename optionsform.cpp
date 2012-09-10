// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QFileInfo>
#include <QFileDialog>
#include <QtCore/QDebug>
#include <QThread>
#include <QMessageBox>

#include "breedersettings.h"
#include "optionsform.h"
#include "ui_optionsform.h"

/*
 * %1 = original image filename without suffix
 * %2 = generations
 * %3 = selected generations
 */
const QString OptionsForm::SaveFilenameTemplate = "%1-%2-%3.png";


OptionsForm::OptionsForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::OptionsForm)
{
    ui->setupUi(this);

    ui->priorityComboBox->addItem(tr("highest"), QThread::HighestPriority);
    ui->priorityComboBox->addItem(tr("high"), QThread::HighPriority);
    ui->priorityComboBox->addItem(tr("normal"), QThread::NormalPriority);
    ui->priorityComboBox->addItem(tr("low"), QThread::LowPriority);
    ui->priorityComboBox->addItem(tr("lowest"), QThread::LowestPriority);
    ui->priorityComboBox->setCurrentIndex(2);

    QObject::connect(ui->priorityComboBox, SIGNAL(currentIndexChanged(int)), SLOT(priorityChanged(int)));
    QObject::connect(ui->selectImageDirectoryPushButton, SIGNAL(clicked()), SLOT(selectImageSaveDirectory()));
    QObject::connect(ui->selectDNADirectoryPushButton, SIGNAL(clicked()), SLOT(selectDNASaveDirectory()));

    QObject::connect(ui->selectLogFilePushButton, SIGNAL(clicked()), SLOT(selectLogFile()));

    // expose signals from internal UI objects
    QObject::connect(ui->saveIntervalSpinBox, SIGNAL(valueChanged(int)), SIGNAL(autoSaveIntervalChanged(int)));
    QObject::connect(ui->autoSaveCheckBox, SIGNAL(toggled(bool)), SIGNAL(autoSaveToggled(bool)));

    // save form values into global settings object
    QObject::connect(ui->colorMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setColorMutationProbability(int)));
    QObject::connect(ui->pointMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointMutationProbability(int)));
    QObject::connect(ui->pointKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointKillProbability(int)));
    QObject::connect(ui->pointEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointEmergenceProbability(int)));
    QObject::connect(ui->genomeKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeKillProbability(int)));
    QObject::connect(ui->genomeMoveProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeMoveProbability(int)));
    QObject::connect(ui->genomeEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeEmergenceProbability(int)));
    QObject::connect(ui->minGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinGenomes(int)));
    QObject::connect(ui->maxGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxGenomes(int)));
    QObject::connect(ui->minPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinPointsPerGenome(int)));
    QObject::connect(ui->maxPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxPointsPerGenome(int)));
    QObject::connect(ui->minAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinA(int)));
    QObject::connect(ui->maxAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxA(int)));
    QObject::connect(ui->startDistributionComboBox, SIGNAL(currentIndexChanged(int)), &gBreederSettings, SLOT(setStartDistribution(int)));
    QObject::connect(ui->scatterFactorSpinBox, SIGNAL(valueChanged(double)), &gBreederSettings, SLOT(setScatterFactor(double)));

    QObject::connect(ui->gpuComputingCheckBox, SIGNAL(toggled(bool)), &gBreederSettings, SLOT(setGPUComputing(bool)));
    QObject::connect(ui->coresSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setCores(int)));

    QObject::connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetToDefaults()));


}


OptionsForm::~OptionsForm()
{
    delete ui;
}


void OptionsForm::go(const QString& where, const QString& what)
{
    show();
    raise();
    if (where == "Autosave") {
        ui->autoSaveTabWidget->setCurrentIndex(1);
        if (what == "imageSaveDirectory") {
            // TODO: show tooltip for field or otherwise highlight it
        }
        else if ((what == "dnaSaveDirectory")) {
            // TODO: show tooltip for field or otherwise highlight it
        }
        else if ((what == "logFile")) {
            // TODO: show tooltip for field or otherwise highlight it
        }
    }
}


void OptionsForm::resetToDefaults(void)
{
    if (QMessageBox::question(this, tr("Reset parameters to defaults?"), tr("Do you really want to reset all parameters to their defaults?")) != QMessageBox::Ok)
        return;
    ui->colorMutationProbabilitySpinBox->setValue(1000);
    ui->pointMutationProbabilitySpinBox->setValue(1000);
    ui->pointKillProbabilitySpinBox->setValue(1000);
    ui->pointEmergenceProbabilitySpinBox->setValue(1000);
    ui->genomeKillProbabilitySpinBox->setValue(1000);
    ui->genomeMoveProbabilitySpinBox->setValue(1000);
    ui->genomeEmergenceProbabilitySpinBox->setValue(10000);
    ui->minGenomesSpinBox->setValue(150);
    ui->maxGenomesSpinBox->setValue(500);
    ui->minPointsSpinBox->setValue(3);
    ui->maxPointsSpinBox->setValue(6);
    ui->minAlphaSpinBox->setValue(5);
    ui->maxAlphaSpinBox->setValue(45);
    ui->startDistributionComboBox->setCurrentIndex(4);
    ui->scatterFactorSpinBox->setValue(0.5);
    ui->coresSpinBox->setValue(QThread::idealThreadCount());
}


void OptionsForm::priorityChanged(int index)
{
    Q_ASSERT(index >= 0 && index < 5);
    static const QThread::Priority mapping[5] = {
        QThread::HighestPriority,
        QThread::HighPriority,
        QThread::NormalPriority,
        QThread::LowPriority,
        QThread::LowestPriority
    };
    emit priorityChanged(mapping[index]);
}


void OptionsForm::selectImageSaveDirectory(void)
{
    const QString& dirName = QFileDialog::getExistingDirectory(this, tr("Choose image save directory"), ui->imageSaveDirectoryLineEdit->text());
    if (dirName != "")
        setImageSaveDirectory(dirName);
}


void OptionsForm::selectDNASaveDirectory(void)
{
    const QString& dirName = QFileDialog::getExistingDirectory(this, tr("Choose DNA save directory"), ui->dnaSaveDirectoryLineEdit->text());
    if (dirName != "")
        setDNASaveDirectory(dirName);
}


void OptionsForm::selectLogFile(void)
{
    const QString& fileName = QFileDialog::getSaveFileName(this, tr("Choose log file"), ui->logFileLineEdit->text());
    if (fileName != "")
        setLogFile(fileName);
}


void OptionsForm::setColorMutationProbability(int v)
{
    ui->colorMutationProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointMutationProbability(int v)
{
    ui->pointMutationProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointEmergenceProbability(int v)
{
    ui->pointEmergenceProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointKillProbability(int v)
{
    ui->pointKillProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGenomeEmergenceProbability(int v)
{
    ui->genomeEmergenceProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGenomeMoveProbability(int v)
{
    ui->genomeMoveProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGenomeKillProbability(int v)
{
    ui->genomeKillProbabilitySpinBox->setValue(v);
}


void OptionsForm::setMinPointsPerGenome(int v)
{
    ui->minPointsSpinBox->setValue(v);
}


void OptionsForm::setMaxPointsPerGenome(int v)
{
    ui->maxPointsSpinBox->setValue(v);
}


void OptionsForm::setMinGenomes(int v)
{
    ui->minGenomesSpinBox->setValue(v);
}


void OptionsForm::setMaxGenomes(int v)
{
    ui->maxGenomesSpinBox->setValue(v);
}


void OptionsForm::setMinAlpha(int v)
{
    ui->minAlphaSpinBox->setValue(v);
}


void OptionsForm::setMaxAlpha(int v)
{
    ui->maxAlphaSpinBox->setValue(v);
}


void OptionsForm::setScatterFactor(qreal v)
{
    Q_ASSERT(v > 0);
    ui->scatterFactorSpinBox->setValue(v);
}


void OptionsForm::setCores(int v)
{
    Q_ASSERT(v > 0);
    ui->coresSpinBox->setValue(v);
}


void OptionsForm::setStartDistribution(int index)
{
    Q_ASSERT(index >= 0);
    ui->startDistributionComboBox->setCurrentIndex(index);
}


void OptionsForm::setSaveInterval(int interval)
{
    ui->saveIntervalSpinBox->setValue(interval);
}


void OptionsForm::setAutoSave(bool checked)
{
    ui->autoSaveCheckBox->setChecked(checked);
}


void OptionsForm::setImageSaveDirectory(const QString& directory)
{
    ui->imageSaveDirectoryLineEdit->setText(directory);
}


void OptionsForm::setDNASaveDirectory(const QString& directory)
{
    ui->dnaSaveDirectoryLineEdit->setText(directory);
}


void OptionsForm::setImageSaveFilenameTemplate(const QString& filenameTemplate)
{
    ui->imageFilenameTemplateLineEdit->setText(filenameTemplate);
}


void OptionsForm::setDNASaveFilenameTemplate(const QString& filenameTemplate)
{
    ui->dnaFilenameTemplateLineEdit->setText(filenameTemplate);
}


void OptionsForm::setLogFile(const QString& filename)
{
    ui->logFileLineEdit->setText(filename);
}


QString OptionsForm::imageFilename(const QString& originalImageFilename, unsigned int generations, unsigned int selected)
{
    QFileInfo fileInfo(originalImageFilename);
    return ui->imageSaveDirectoryLineEdit->text() + "/" + QString(ui->imageFilenameTemplateLineEdit->text()).arg(fileInfo.completeBaseName()).arg(generations, 10, 10, QChar('0')).arg(selected, 9, 10, QChar('0'));
}


QString OptionsForm::dnaFilename(const QString& originalImageFilename, unsigned int generations, unsigned int selected)
{
    QFileInfo fileInfo(originalImageFilename);
    return ui->dnaSaveDirectoryLineEdit->text() + "/" + QString(ui->dnaFilenameTemplateLineEdit->text()).arg(fileInfo.completeBaseName()).arg(generations, 10, 10, QChar('0')).arg(selected, 9, 10, QChar('0'));
}




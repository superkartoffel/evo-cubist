// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QFileInfo>
#include <QFileDialog>
#include <QtCore/QDebug>

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

    QObject::connect(ui->selectImageDirectoryPushButton, SIGNAL(clicked()), SLOT(selectImageSaveDirectory()));
    QObject::connect(ui->selectDNADirectoryPushButton, SIGNAL(clicked()), SLOT(selectDNASaveDirectory()));

    // expose signals from internal UI objects
    QObject::connect(ui->saveIntervalSpinBox, SIGNAL(valueChanged(int)), SIGNAL(autoSaveIntervalChanged(int)));
    QObject::connect(ui->autoSaveCheckBox, SIGNAL(toggled(bool)), SIGNAL(autoSaveToggled(bool)));

    // save form values into global settings object
    QObject::connect(ui->colorMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setColorMutationProbability(int)));
    QObject::connect(ui->pointMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointMutationProbability(int)));
    QObject::connect(ui->pointKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointKillProbability(int)));
    QObject::connect(ui->pointEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointEmergenceProbability(int)));
    QObject::connect(ui->genomeKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeKillProbability(int)));
    QObject::connect(ui->genomeEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeEmergenceProbability(int)));
    QObject::connect(ui->minGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinGenomes(int)));
    QObject::connect(ui->maxGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxGenomes(int)));
    QObject::connect(ui->minPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinPointsPerGenome(int)));
    QObject::connect(ui->maxPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxPointsPerGenome(int)));
    QObject::connect(ui->minAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinA(int)));
    QObject::connect(ui->maxAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxA(int)));
    QObject::connect(ui->parallelComputingCheckBox, SIGNAL(toggled(bool)), &gBreederSettings, SLOT(setParallelComputing(bool)));
    QObject::connect(ui->gpuComputingCheckBox, SIGNAL(toggled(bool)), &gBreederSettings, SLOT(setGPUComputing(bool)));
}


OptionsForm::~OptionsForm()
{
    delete ui;
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


void OptionsForm::setColorMutationProbability(int v)
{
    ui->colorMutationProbabilitySpinBox->blockSignals(true);
    ui->colorMutationProbabilitySpinBox->setValue(v);
    ui->colorMutationProbabilitySpinBox->blockSignals(false);
}


void OptionsForm::setPointMutationProbability(int v)
{
    ui->pointMutationProbabilitySpinBox->blockSignals(true);
    ui->pointMutationProbabilitySpinBox->setValue(v);
    ui->pointMutationProbabilitySpinBox->blockSignals(false);
}


void OptionsForm::setPointEmergenceProbability(int v)
{
    ui->pointEmergenceProbabilitySpinBox->blockSignals(true);
    ui->pointEmergenceProbabilitySpinBox->setValue(v);
    ui->pointEmergenceProbabilitySpinBox->blockSignals(false);
}


void OptionsForm::setPointKillProbability(int v)
{
    ui->pointKillProbabilitySpinBox->blockSignals(true);
    ui->pointKillProbabilitySpinBox->setValue(v);
    ui->pointKillProbabilitySpinBox->blockSignals(false);
}


void OptionsForm::setGenomeEmergenceProbability(int v)
{
    ui->genomeEmergenceProbabilitySpinBox->blockSignals(true);
    ui->genomeEmergenceProbabilitySpinBox->setValue(v);
    ui->genomeEmergenceProbabilitySpinBox->blockSignals(false);
}


void OptionsForm::setGenomeKillProbability(int v)
{
    ui->genomeKillProbabilitySpinBox->blockSignals(true);
    ui->genomeKillProbabilitySpinBox->setValue(v);
    ui->genomeKillProbabilitySpinBox->blockSignals(false);
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



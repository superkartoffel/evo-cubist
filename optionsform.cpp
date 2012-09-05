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

    QObject::connect(ui->colorMutationRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setColorMutationRate(int)));
    QObject::connect(ui->pointMutationRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointMutationRate(int)));
    QObject::connect(ui->pointKillRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointKillRate(int)));
    QObject::connect(ui->pointEmergenceRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setPointEmergenceRate(int)));
    QObject::connect(ui->genomeKillRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeKillRate(int)));
    QObject::connect(ui->genomeEmergenceRateSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setGenomeEmergenceRate(int)));
    QObject::connect(ui->minGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinGenomes(int)));
    QObject::connect(ui->maxGenomesSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxGenomes(int)));
    QObject::connect(ui->minPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinPointsPerGenome(int)));
    QObject::connect(ui->maxPointsSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxPointsPerGenome(int)));
    QObject::connect(ui->minAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMinA(int)));
    QObject::connect(ui->maxAlphaSpinBox, SIGNAL(valueChanged(int)), &gBreederSettings, SLOT(setMaxA(int)));
}


OptionsForm::~OptionsForm()
{
    delete ui;
}


int OptionsForm::saveInterval(void) const
{
    return ui->saveIntervalSpinBox->value();
}


bool OptionsForm::autoSave(void) const
{
    return ui->autoSaveCheckBox->isChecked();
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


QString OptionsForm::imageSaveDirectory(void) const
{
    return ui->imageSaveDirectoryLineEdit->text();
}


QString OptionsForm::imageSaveFilenameTemplate(void) const
{
    return ui->imageFilenameTemplateLineEdit->text();
}


QString OptionsForm::dnaSaveDirectory(void) const
{
    return ui->dnaSaveDirectoryLineEdit->text();
}


QString OptionsForm::dnaSaveFilenameTemplate(void) const
{
    return ui->dnaFilenameTemplateLineEdit->text();
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


void OptionsForm::setColorMutationRate(int v)
{
    ui->colorMutationRateSpinBox->blockSignals(true);
    ui->colorMutationRateSpinBox->setValue(v);
    ui->colorMutationRateSpinBox->blockSignals(false);
}


void OptionsForm::setPointMutationRate(int v)
{
    ui->pointMutationRateSpinBox->blockSignals(true);
    ui->pointMutationRateSpinBox->setValue(v);
    ui->pointMutationRateSpinBox->blockSignals(false);
}


void OptionsForm::setPointEmergenceRate(int v)
{
    ui->pointEmergenceRateSpinBox->blockSignals(true);
    ui->pointEmergenceRateSpinBox->setValue(v);
    ui->pointEmergenceRateSpinBox->blockSignals(false);
}


void OptionsForm::setPointKillRate(int v)
{
    ui->pointKillRateSpinBox->blockSignals(true);
    ui->pointKillRateSpinBox->setValue(v);
    ui->pointKillRateSpinBox->blockSignals(false);
}


void OptionsForm::setGenomeEmergenceRate(int v)
{
    ui->genomeEmergenceRateSpinBox->blockSignals(true);
    ui->genomeEmergenceRateSpinBox->setValue(v);
    ui->genomeEmergenceRateSpinBox->blockSignals(false);
}


void OptionsForm::setGenomeKillRate(int v)
{
    ui->genomeKillRateSpinBox->blockSignals(true);
    ui->genomeKillRateSpinBox->setValue(v);
    ui->genomeKillRateSpinBox->blockSignals(false);
}

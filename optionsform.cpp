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

    QObject::connect(ui->selectImageDirectoryPushButton, SIGNAL(clicked()), SLOT(selectImageSaveDirectory()));
    QObject::connect(ui->selectDNADirectoryPushButton, SIGNAL(clicked()), SLOT(selectDNASaveDirectory()));

    QObject::connect(ui->selectLogFilePushButton, SIGNAL(clicked()), SLOT(selectLogFile()));

    // expose signals from internal UI objects
    QObject::connect(ui->saveIntervalSpinBox, SIGNAL(valueChanged(int)), SIGNAL(autoSaveIntervalChanged(int)));
    QObject::connect(ui->autoSaveCheckBox, SIGNAL(toggled(bool)), SIGNAL(autoSaveToggled(bool)));

    // save form values into global settings object
    QObject::connect(ui->colorMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setColorMutationProbability(int)));
    QObject::connect(ui->pointMutationProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setPointMutationProbability(int)));
    QObject::connect(ui->pointKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setPointKillProbability(int)));
    QObject::connect(ui->pointEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setPointEmergenceProbability(int)));
    QObject::connect(ui->geneKillProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setGeneKillProbability(int)));
    QObject::connect(ui->geneMoveProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setGeneMoveProbability(int)));
    QObject::connect(ui->geneEmergenceProbabilitySpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setGeneEmergenceProbability(int)));
    QObject::connect(ui->minGenesSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMinGenes(int)));
    QObject::connect(ui->minGenesSpinBox, SIGNAL(valueChanged(int)), SLOT(minGenesChanged(int)));
    QObject::connect(ui->maxGenesSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMaxGenes(int)));
    QObject::connect(ui->maxGenesSpinBox, SIGNAL(valueChanged(int)), SLOT(maxGenesChanged(int)));
    QObject::connect(ui->minPointsSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMinPointsPerGene(int)));
    QObject::connect(ui->minPointsSpinBox, SIGNAL(valueChanged(int)), SLOT(minPointsPerGeneChanged(int)));
    QObject::connect(ui->maxPointsSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMaxPointsPerGene(int)));
    QObject::connect(ui->maxPointsSpinBox, SIGNAL(valueChanged(int)), SLOT(maxPointsPerGeneChanged(int)));
    QObject::connect(ui->minAlphaSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMinA(int)));
    QObject::connect(ui->minAlphaSpinBox, SIGNAL(valueChanged(int)), SLOT(minAlphaChanged(int)));
    QObject::connect(ui->maxAlphaSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setMaxA(int)));
    QObject::connect(ui->maxAlphaSpinBox, SIGNAL(valueChanged(int)), SLOT(maxAlphaChanged(int)));
    QObject::connect(ui->startDistributionComboBox, SIGNAL(currentIndexChanged(int)), &gSettings, SLOT(setStartDistribution(int)));
    QObject::connect(ui->startDistributionComboBox, SIGNAL(currentIndexChanged(int)), SLOT(startDistributionChanged(int)));
    QObject::connect(ui->scatterFactorSpinBox, SIGNAL(valueChanged(double)), &gSettings, SLOT(setScatterFactor(double)));
    QObject::connect(ui->coresSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setCores(int)));
    QObject::connect(ui->saveIntervalSpinBox, SIGNAL(valueChanged(int)), &gSettings, SLOT(setAutoSaveInterval(int)));
    QObject::connect(ui->autoSaveCheckBox, SIGNAL(toggled(bool)), &gSettings, SLOT(setAutoSave(bool)));
    QObject::connect(ui->imageSaveDirectoryLineEdit, SIGNAL(textChanged(QString)), &gSettings, SLOT(setImageSaveDirectory(QString)));
    QObject::connect(ui->dnaSaveDirectoryLineEdit, SIGNAL(textChanged(QString)), &gSettings, SLOT(setDNASaveDirectory(QString)));
    QObject::connect(ui->imageFilenameTemplateLineEdit, SIGNAL(textChanged(QString)), &gSettings, SLOT(setImageSaveFilenameTemplate(QString)));
    QObject::connect(ui->dnaFilenameTemplateLineEdit, SIGNAL(textChanged(QString)), &gSettings, SLOT(setDNASaveFilenameTemplate(QString)));
//  QObject::connect(ui->gpuComputingCheckBox, SIGNAL(toggled(bool)), &gSettings, SLOT(setGPUComputing(bool)));

    QObject::connect(ui->resetPushButton, SIGNAL(clicked()), SLOT(resetToDefaults()));
}


OptionsForm::~OptionsForm()
{
    delete ui;
}


void OptionsForm::showEvent(QShowEvent*)
{
    raise();
}


void OptionsForm::go(const QString& where, const QString& what)
{
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
    show();
}


void OptionsForm::minGenesChanged(int v)
{
    ui->maxGenesSpinBox->setMinimum(v);
}


void OptionsForm::maxGenesChanged(int v)
{
    ui->minGenesSpinBox->setMaximum(v);
}


void OptionsForm::minPointsPerGeneChanged(int v)
{
    ui->maxPointsSpinBox->setMinimum(v);
}


void OptionsForm::maxPointsPerGeneChanged(int v)
{
    ui->minPointsSpinBox->setMaximum(v);
}


void OptionsForm::minAlphaChanged(int v)
{
    ui->maxAlphaSpinBox->setMinimum(v);
}


void OptionsForm::maxAlphaChanged(int v)
{
    ui->minAlphaSpinBox->setMaximum(v);
}


void OptionsForm::startDistributionChanged(int index)
{
    ui->scatterFactorSpinBox->setEnabled(index == 3 || index == 4);
}


void OptionsForm::resetToDefaults(void)
{
    if (QMessageBox::question(this, tr("Reset parameters to defaults?"), tr("Do you really want to reset all parameters to their defaults?"), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel)
        return;
    ui->colorMutationProbabilitySpinBox->setValue(1000);
    ui->pointMutationProbabilitySpinBox->setValue(1000);
    ui->pointKillProbabilitySpinBox->setValue(1000);
    ui->pointEmergenceProbabilitySpinBox->setValue(1000);
    ui->geneKillProbabilitySpinBox->setValue(1000);
    ui->geneMoveProbabilitySpinBox->setValue(1000);
    ui->geneEmergenceProbabilitySpinBox->setValue(10000);
    ui->minGenesSpinBox->setValue(150);
    ui->maxGenesSpinBox->setValue(500);
    ui->minPointsSpinBox->setValue(3);
    ui->maxPointsSpinBox->setValue(6);
    ui->minAlphaSpinBox->setValue(5);
    ui->maxAlphaSpinBox->setValue(45);
    ui->startDistributionComboBox->setCurrentIndex(4);
    ui->scatterFactorSpinBox->setValue(0.5);
    ui->coresSpinBox->setValue(QThread::idealThreadCount());
}


void OptionsForm::selectImageSaveDirectory(void)
{
    const QString& dirName = QFileDialog::getExistingDirectory(this, tr("Choose image save directory"), ui->imageSaveDirectoryLineEdit->text());
    if (!dirName.isNull())
        setImageSaveDirectory(dirName);
}


void OptionsForm::selectDNASaveDirectory(void)
{
    const QString& dirName = QFileDialog::getExistingDirectory(this, tr("Choose DNA save directory"), ui->dnaSaveDirectoryLineEdit->text());
    if (!dirName.isNull())
        setDNASaveDirectory(dirName);
}


void OptionsForm::selectLogFile(void)
{
    const QString& fileName = QFileDialog::getSaveFileName(this, tr("Choose log file"), ui->logFileLineEdit->text());
    if (!fileName.isNull())
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


void OptionsForm::setGeneEmergenceProbability(int v)
{
    ui->geneEmergenceProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGeneMoveProbability(int v)
{
    ui->geneMoveProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGeneKillProbability(int v)
{
    ui->geneKillProbabilitySpinBox->setValue(v);
}


void OptionsForm::setMinPointsPerGene(int v)
{
    ui->minPointsSpinBox->setValue(v);
}


void OptionsForm::setMaxPointsPerGene(int v)
{
    ui->maxPointsSpinBox->setValue(v);
}


void OptionsForm::setMinGenes(int v)
{
    ui->minGenesSpinBox->setValue(v);
}


void OptionsForm::setMaxGenes(int v)
{
    ui->maxGenesSpinBox->setValue(v);
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




// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QFileInfo>
#include <QFileDialog>
#include <QtCore/QDebug>
#include <QThread>
#include <QMessageBox>
#include <QStandardItemModel>

#include "main.h"
#include "breedersettings.h"
#include "optionsform.h"
#include "helper.h"
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
    setWindowTitle(QString("%1 - Options").arg(AppName));

    QObject::connect(ui->selectImageDirectoryPushButton, SIGNAL(clicked()), SLOT(selectImageSaveDirectory()));
    QObject::connect(ui->selectDNADirectoryPushButton, SIGNAL(clicked()), SLOT(selectDNASaveDirectory()));

    QObject::connect(ui->selectLogFilePushButton, SIGNAL(clicked()), SLOT(selectLogFile()));

    // expose signals from internal UI objects
    QObject::connect(ui->saveIntervalSpinBox, SIGNAL(valueChanged(int)), SIGNAL(autoSaveIntervalChanged(int)));
    QObject::connect(ui->autoSaveCheckBox, SIGNAL(toggled(bool)), SIGNAL(autoSaveToggled(bool)));

    QObject::connect(ui->backgroundColorLineEdit, SIGNAL(textChanged(const QString&)), SLOT(backgroundColorChanged(const QString&)));
    QObject::connect(ui->setBackgroundColorPushButton, SIGNAL(clicked()), SLOT(backgroundColorSelected()));

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
    QObject::connect(ui->internalLogCheckBox, SIGNAL(toggled(bool)), SLOT(internalLogToggled(bool)));
}


OptionsForm::~OptionsForm()
{
    delete ui;
}


void OptionsForm::showEvent(QShowEvent*)
{
    activateWindow();
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
    Q_ASSERT(v >= 0);
    ui->maxGenesSpinBox->setMinimum(v);
}


void OptionsForm::maxGenesChanged(int v)
{
    Q_ASSERT(v >= 0);
    ui->minGenesSpinBox->setMaximum(v);
}


void OptionsForm::setStartDistributionComboBoxItemEnabled(int idx, bool enabled)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->startDistributionComboBox->model());
    if (model) {
        QModelIndex index = model->index(idx, ui->startDistributionComboBox->modelColumn(), ui->startDistributionComboBox->rootModelIndex());
        QStandardItem* item = model->itemFromIndex(index);
        if (item)
            item->setEnabled(enabled);
    }
}


void OptionsForm::minPointsPerGeneChanged(int v)
{
    Q_ASSERT(v >= 3);
    ui->maxPointsSpinBox->setMinimum(v);
    setStartDistributionComboBoxItemEnabled(TiledDistribution, v > 3);
    setStartDistributionComboBoxItemEnabled(TiledWithColorHintDistribution, v > 3);
}


void OptionsForm::maxPointsPerGeneChanged(int v)
{
    Q_ASSERT(v >= 3);
    Q_ASSERT(v >= ui->minPointsSpinBox->value());
    ui->minPointsSpinBox->setMaximum(v);
}


void OptionsForm::minAlphaChanged(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    ui->maxAlphaSpinBox->setMinimum(v);
}


void OptionsForm::maxAlphaChanged(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    ui->minAlphaSpinBox->setMaximum(v);
}


void OptionsForm::startDistributionChanged(int index)
{
    ui->scatterFactorSpinBox->setEnabled(index == ScatteredDistribution || index == ScatteredWithColorHintDistribution);
    emit startDistributionChanged();
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
    Q_ASSERT(v > 0);
    ui->colorMutationProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointMutationProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->pointMutationProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointEmergenceProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->pointEmergenceProbabilitySpinBox->setValue(v);
}


void OptionsForm::setPointKillProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->pointKillProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGeneEmergenceProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->geneEmergenceProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGeneMoveProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->geneMoveProbabilitySpinBox->setValue(v);
}


void OptionsForm::setGeneKillProbability(int v)
{
    Q_ASSERT(v > 0);
    ui->geneKillProbabilitySpinBox->setValue(v);
}


void OptionsForm::setMinPointsPerGene(int v)
{
    Q_ASSERT(v >= 3);
    ui->minPointsSpinBox->setValue(v);
}


void OptionsForm::setMaxPointsPerGene(int v)
{
    Q_ASSERT(v >= 3);
    ui->maxPointsSpinBox->setValue(v);
}


void OptionsForm::setMinGenes(int v)
{
    Q_ASSERT(v > 0);
    ui->minGenesSpinBox->setValue(v);
}


void OptionsForm::setMaxGenes(int v)
{
    Q_ASSERT(v >= 0);
    ui->maxGenesSpinBox->setValue(v);
}


void OptionsForm::setMinAlpha(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
    ui->minAlphaSpinBox->setValue(v);
}


void OptionsForm::setMaxAlpha(int v)
{
    Q_ASSERT(v >= 0);
    Q_ASSERT(v < 256);
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


void OptionsForm::setInternalLogEnabled(bool enabled)
{
    ui->internalLogCheckBox->setChecked(enabled);
}


void OptionsForm::setStartDistribution(int index)
{
    Q_ASSERT(index >= 0);
    ui->startDistributionComboBox->blockSignals(true);
    ui->startDistributionComboBox->setCurrentIndex(index);
    ui->startDistributionComboBox->blockSignals(false);
}


void OptionsForm::setSaveInterval(int interval)
{
    Q_ASSERT(interval > 0);
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


void OptionsForm::backgroundColorChanged(const QString& colorString)
{
    const QColor newColor(colorString);
    if (newColor.isValid()) {
        ui->backgroundColorFrame->setStyleSheet(QString("background-color: %1").arg(colorString));
        ui->setBackgroundColorPushButton->setEnabled(true);
    }
    else {
        ui->backgroundColorFrame->setStyleSheet("background-image: url(:/images/checkered-pattern.png)");
        ui->setBackgroundColorPushButton->setEnabled(false);
    }
}


void OptionsForm::backgroundColorChanged(QRgb rgb)
{
    const QString& colorString = QString("#%1%2%3")
            .arg(qRed(rgb), 2, 16, QChar('0'))
            .arg(qGreen(rgb), 2, 16, QChar('0'))
            .arg(qBlue(rgb), 2, 16, QChar('0'));
    ui->backgroundColorLineEdit->setText(colorString);
    ui->backgroundColorFrame->setStyleSheet(QString("background-color: %1").arg(colorString));
}


void OptionsForm::backgroundColorSelected(void)
{
    const QColor newColor(ui->backgroundColorLineEdit->text());
    if (newColor.isValid()) {
        const QRgb rgb = newColor.rgba();
        gSettings.setBackgroundColor(rgb);
        emit backgroundColorSelected(rgb);
    }
}


void OptionsForm::internalLogToggled(bool checked)
{
    if (checked && sender() != NULL) {
        const int rc = QMessageBox::warning(this, tr("Excessive memory usage warning"), tr("Attention, enabling internal logging uses up more and more memory as time goes by. Are you sure you want to enable the log?"), QMessageBox::Yes, QMessageBox::No);
        if (rc != QMessageBox::Yes)
            ui->internalLogCheckBox->setChecked(false);
    }
}


QString OptionsForm::makeImageFilename(const QString& originalImageFilename, unsigned int generations, unsigned int selected)
{
    QFileInfo fileInfo(originalImageFilename);
    const QString& filename = ui->imageSaveDirectoryLineEdit->text() + "/" + QString(ui->imageFilenameTemplateLineEdit->text()).arg(fileInfo.completeBaseName()).arg(generations, 10, 10, QChar('0')).arg(selected, 9, 10, QChar('0'));
    return filename;
}


QString OptionsForm::makeDNAFilename(const QString& originalImageFilename, unsigned int generations, unsigned int selected)
{
    QFileInfo fileInfo(originalImageFilename);
    const QString& filename = ui->dnaSaveDirectoryLineEdit->text() + "/" + QString(ui->dnaFilenameTemplateLineEdit->text()).arg(fileInfo.completeBaseName()).arg(generations, 10, 10, QChar('0')).arg(selected, 9, 10, QChar('0'));
    return filename;
}

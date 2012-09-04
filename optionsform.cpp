// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QFileInfo>
#include <QFileDialog>
#include <QtCore/QDebug>

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

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
    QObject::connect(ui->selectDirectoryPushButton, SIGNAL(clicked()), SLOT(selectSaveDirectory()));
}


OptionsForm::~OptionsForm()
{
    delete ui;
}


void OptionsForm::selectSaveDirectory(void)
{
    const QString& dirName = QFileDialog::getExistingDirectory(this, tr("Choose save directory"), ui->saveDirectoryLineEdit->text());
    if (dirName != "")
        setSaveDirectory(dirName);
}


int OptionsForm::saveInterval(void) const
{
    return ui->saveIntervalSpinBox->value();
}


bool OptionsForm::autoSave(void) const
{
    return ui->autoSaveCheckBox->isChecked();
}


QString OptionsForm::saveDirectory(void) const
{
    return ui->saveDirectoryLineEdit->text();
}


QString OptionsForm::saveFilenameTemplate(void) const
{
    return ui->imageFilenameTemplateLineEdit->text();
}


void OptionsForm::setSaveInterval(int interval)
{
    ui->saveIntervalSpinBox->setValue(interval);
}


void OptionsForm::setAutoSave(bool checked)
{
    ui->autoSaveCheckBox->setChecked(checked);
}


void OptionsForm::setSaveDirectory(const QString& directory)
{
    ui->saveDirectoryLineEdit->setText(directory);
}


void OptionsForm::setSaveFilenameTemplate(const QString& filenameTemplate)
{
    ui->imageFilenameTemplateLineEdit->setText(filenameTemplate);
}


QString OptionsForm::filenameFromImageFilename(const QString& imageFilename, unsigned int generations, unsigned int selected)
{
    QFileInfo fileInfo(imageFilename);
    const QString& filename = fileInfo.completeBaseName();
    return ui->saveDirectoryLineEdit->text() + "/" + QString(ui->imageFilenameTemplateLineEdit->text()).arg(filename).arg(generations, 10, 10, QChar('0')).arg(selected, 9, 10, QChar('0'));
}

// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include <QFileInfo>

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
    , mGenerations(0U)
    , mSelected(0U)
    , mSaveFilenameTemplate(SaveFilenameTemplate)
{
    ui->setupUi(this);
}


OptionsForm::~OptionsForm()
{
    delete ui;
}


void OptionsForm::setSaveDirectory(const QString& directory)
{
    mSaveDirectory = directory;
}


void OptionsForm::setSaveFilenameTemplate(const QString& filenameTemplate)
{
    mSaveFilenameTemplate = filenameTemplate;
}


void OptionsForm::setImageFilename(const QString& imageFilename)
{
    QFileInfo fileInfo(imageFilename);
    const QString& filename = fileInfo.completeBaseName().remove(fileInfo.suffix());
    mSaveFilename = mSaveDirectory + QString(mSaveFilenameTemplate).arg(filename).arg(mGenerations).arg(mSelected);
}


void OptionsForm::setGenerations(unsigned int generations, unsigned int selected)
{
    mGenerations = generations;
    mSelected = selected;
}

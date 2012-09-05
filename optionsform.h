// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __OPTIONSFORM_H_
#define __OPTIONSFORM_H_

#include <QWidget>
#include "breedersettings.h"
#include "ui_optionsform.h"


namespace Ui {
class OptionsForm;
}

class OptionsForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit OptionsForm(QWidget* parent = NULL);
    ~OptionsForm();

    QString imageSaveDirectory(void) const;
    QString imageSaveFilenameTemplate(void) const;
    QString dnaSaveDirectory(void) const;
    QString dnaSaveFilenameTemplate(void) const;
    int saveInterval(void) const;
    bool autoSave(void) const;

    void setSaveInterval(int);
    void setAutoSave(bool);
    void setImageSaveDirectory(const QString&);
    void setImageSaveFilenameTemplate(const QString&);
    void setDNASaveDirectory(const QString&);
    void setDNASaveFilenameTemplate(const QString&);
    QString imageFilename(const QString&, unsigned int generations, unsigned int selected);
    QString dnaFilename(const QString&, unsigned int generations, unsigned int selected);

    inline int colorMutationRate(void) const { return ui->colorMutationRateSpinBox->value(); }
    inline int pointMutationRate(void) const { return ui->pointMutationRateSpinBox->value(); }
    inline int pointEmergenceRate(void) const { return ui->pointEmergenceRateSpinBox->value(); }
    inline int pointKillRate(void) const { return ui->pointKillRateSpinBox->value(); }
    inline int genomeEmergenceRate(void) const { return ui->genomeEmergenceRateSpinBox->value(); }
    inline int genomeKillRate(void) const { return ui->genomeKillRateSpinBox->value(); }

    void setColorMutationRate(int);
    void setPointMutationRate(int);
    void setPointEmergenceRate(int);
    void setPointKillRate(int);
    void setGenomeEmergenceRate(int);
    void setGenomeKillRate(int);

signals:


private slots:
    void selectImageSaveDirectory(void);
    void selectDNASaveDirectory(void);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;


};

#endif // __OPTIONSFORM_H_

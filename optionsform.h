// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __OPTIONSFORM_H_
#define __OPTIONSFORM_H_

#include <QWidget>
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

    inline unsigned int colorMutationRate(void) const { return (unsigned int)ui->colorMutationRateSpinBox->value(); }
    inline unsigned int pointMutationRate(void) const { return (unsigned int)ui->pointMutationRateSpinBox->value(); }
    inline unsigned int pointEmergenceRate(void) const { return (unsigned int)ui->pointEmergenceRateSpinBox->value(); }
    inline unsigned int pointKillRate(void) const { return (unsigned int)ui->pointKillRateSpinBox->value(); }
    inline unsigned int genomeEmergenceRate(void) const { return (unsigned int)ui->genomeEmergenceRateSpinBox->value(); }
    inline unsigned int genomeKillRate(void) const { return (unsigned int)ui->genomeKillRateSpinBox->value(); }

    inline void setColorMutationRate(unsigned int r) { ui->colorMutationRateSpinBox->setValue(r); }
    inline void setPointMutationRate(unsigned int r) { ui->pointMutationRateSpinBox->setValue(r); }
    inline void setPointEmergenceRate(unsigned int r) { ui->pointEmergenceRateSpinBox->setValue(r); }
    inline void setPointKillRate(unsigned int r) { ui->pointKillRateSpinBox->setValue(r); }
    inline void setGenomeEmergenceRate(unsigned int r) { ui->genomeEmergenceRateSpinBox->setValue(r); }
    inline void setGenomeKillRate(unsigned int r) { ui->genomeKillRateSpinBox->setValue(r); }

private slots:
    void selectImageSaveDirectory(void);
    void selectDNASaveDirectory(void);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;


};

#endif // __OPTIONSFORM_H_

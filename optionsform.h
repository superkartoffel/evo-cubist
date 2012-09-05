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

    inline int colorMutationProbability(void) const { return ui->colorMutationProbabilitySpinBox->value(); }
    inline int pointMutationProbability(void) const { return ui->pointMutationProbabilitySpinBox->value(); }
    inline int pointEmergenceProbability(void) const { return ui->pointEmergenceProbabilitySpinBox->value(); }
    inline int pointKillProbability(void) const { return ui->pointKillProbabilitySpinBox->value(); }
    inline int genomeEmergenceProbability(void) const { return ui->genomeEmergenceProbabilitySpinBox->value(); }
    inline int genomeKillProbability(void) const { return ui->genomeKillProbabilitySpinBox->value(); }
    inline int minPointsPerGenome(void) const { return ui->minPointsSpinBox->value(); }
    inline int maxPointsPerGenome(void) const { return ui->maxPointsSpinBox->value(); }
    inline int minGenomes(void) const { return ui->minGenomesSpinBox->value(); }
    inline int maxGenomes(void) const { return ui->maxGenomesSpinBox->value(); }
    inline int minAlpha(void) const { return ui->minAlphaSpinBox->value(); }
    inline int maxAlpha(void) const { return ui->maxAlphaSpinBox->value(); }

    void setColorMutationProbability(int);
    void setPointMutationProbability(int);
    void setPointEmergenceProbability(int);
    void setPointKillProbability(int);
    void setGenomeEmergenceProbability(int);
    void setGenomeKillProbability(int);

signals:
    void autoSaveIntervalChanged(int);
    void autoSaveToggled(bool);

private slots:
    void selectImageSaveDirectory(void);
    void selectDNASaveDirectory(void);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;


};

#endif // __OPTIONSFORM_H_

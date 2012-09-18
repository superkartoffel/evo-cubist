// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __OPTIONSFORM_H_
#define __OPTIONSFORM_H_

#include <QWidget>
#include <QThread>
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

    void go(const QString& where, const QString& what);

    void setSaveInterval(int);
    void setAutoSave(bool);
    void setImageSaveDirectory(const QString&);
    void setImageSaveFilenameTemplate(const QString&);
    void setDNASaveDirectory(const QString&);
    void setDNASaveFilenameTemplate(const QString&);
    QString imageFilename(const QString&, unsigned int generations, unsigned int selected);
    QString dnaFilename(const QString&, unsigned int generations, unsigned int selected);

    inline QString imageSaveDirectory(void) const { return ui->imageSaveDirectoryLineEdit->text(); }
    inline QString imageSaveFilenameTemplate(void) const { return ui->imageFilenameTemplateLineEdit->text(); }
    inline QString dnaSaveDirectory(void) const { return ui->dnaSaveDirectoryLineEdit->text(); }
    inline QString dnaSaveFilenameTemplate(void) const { return ui->dnaFilenameTemplateLineEdit->text(); }
    inline int saveInterval(void) const { return ui->saveIntervalSpinBox->value(); }
    inline bool autoSave(void) const { return ui->autoSaveCheckBox->isChecked(); }

    void setLogFile(const QString&);
    QString logFile(void) const { return ui->logFileLineEdit->text(); }

    inline int colorMutationProbability(void) const { return ui->colorMutationProbabilitySpinBox->value(); }
    inline int pointMutationProbability(void) const { return ui->pointMutationProbabilitySpinBox->value(); }
    inline int pointEmergenceProbability(void) const { return ui->pointEmergenceProbabilitySpinBox->value(); }
    inline int pointKillProbability(void) const { return ui->pointKillProbabilitySpinBox->value(); }
    inline int geneEmergenceProbability(void) const { return ui->geneEmergenceProbabilitySpinBox->value(); }
    inline int geneMoveProbability(void) const { return ui->geneMoveProbabilitySpinBox->value(); }
    inline int geneKillProbability(void) const { return ui->geneKillProbabilitySpinBox->value(); }
    inline int minPointsPerGene(void) const { return ui->minPointsSpinBox->value(); }
    inline int maxPointsPerGene(void) const { return ui->maxPointsSpinBox->value(); }
    inline int minGenes(void) const { return ui->minGenesSpinBox->value(); }
    inline int maxGenes(void) const { return ui->maxGenesSpinBox->value(); }
    inline int minAlpha(void) const { return ui->minAlphaSpinBox->value(); }
    inline int maxAlpha(void) const { return ui->maxAlphaSpinBox->value(); }
    inline int startDistribution(void) const { return ui->startDistributionComboBox->currentIndex(); }
    inline qreal scatterFactor(void) const { return ui->scatterFactorSpinBox->value(); }
    inline int cores(void) const { return ui->coresSpinBox->value(); }

    void setColorMutationProbability(int);
    void setPointMutationProbability(int);
    void setPointEmergenceProbability(int);
    void setPointKillProbability(int);
    void setGeneEmergenceProbability(int);
    void setGeneMoveProbability(int);
    void setGeneKillProbability(int);
    void setMinPointsPerGene(int);
    void setMaxPointsPerGene(int);
    void setMinGenes(int);
    void setMaxGenes(int);
    void setMinAlpha(int);
    void setMaxAlpha(int);
    void setStartDistribution(int);
    void setScatterFactor(qreal);
    void setCores(int);

signals:
    void autoSaveIntervalChanged(int);
    void autoSaveToggled(bool);
    void logFileChanged(const QString&);

private slots:
    void selectImageSaveDirectory(void);
    void selectDNASaveDirectory(void);
    void selectLogFile(void);
    void resetToDefaults(void);
    void startDistributionChanged(int);
    void minGenesChanged(int);
    void maxGenesChanged(int);
    void minPointsPerGeneChanged(int);
    void maxPointsPerGeneChanged(int);
    void minAlphaChanged(int);
    void maxAlphaChanged(int);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;


};

#endif // __OPTIONSFORM_H_

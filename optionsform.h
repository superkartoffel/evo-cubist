// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __OPTIONSFORM_H_
#define __OPTIONSFORM_H_

#include <QWidget>

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
    
private slots:
    void selectImageSaveDirectory(void);
    void selectDNASaveDirectory(void);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;
};

#endif // __OPTIONSFORM_H_

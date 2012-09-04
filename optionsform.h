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

    QString saveDirectory(void) const;
    QString saveFilenameTemplate(void) const;
    int saveInterval(void) const;
    bool autoSave(void) const;

    void setSaveInterval(int);
    void setAutoSave(bool);
    void setSaveDirectory(const QString&);
    void setSaveFilenameTemplate(const QString&);
    QString filenameFromImageFilename(const QString&, unsigned int generations, unsigned int selected);
    
private slots:
    void selectSaveDirectory(void);

private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm* ui;
};

#endif // __OPTIONSFORM_H_

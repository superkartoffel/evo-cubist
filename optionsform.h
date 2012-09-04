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

    const QString& saveDirectory(void) const { return mSaveDirectory; }
    const QString& saveFilename(void) const { return mSaveFilename; }
    const QString& saveFilenameTemplate(void) const { return mSaveFilenameTemplate; }

    void setSaveDirectory(const QString&);
    void setSaveFilenameTemplate(const QString&);
    void setImageFilename(const QString&);
    void setGenerations(unsigned int generations, unsigned int selected);
    
private:
    static const QString SaveFilenameTemplate;

    Ui::OptionsForm *ui;

    QString mSaveDirectory;
    QString mSaveFilenameTemplate;
    QString mSaveFilename;
    QString mImageFilename;
    unsigned int mGenerations;
    unsigned int mSelected;
};

#endif // __OPTIONSFORM_H_

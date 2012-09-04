// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QDateTime>
#include <QString>
#include <QTimer>

#include "imagewidget.h"
#include "generationwidget.h"
#include "breeder.h"
#include "optionsform.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    static const QString Company;
    static const QString AppName;
    static const QString AppVersion;

protected:
    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow *ui;
    OptionsForm mOptionsForm;
    ImageWidget* mImageWidget;
    GenerationWidget* mGenerationWidget;
    Breeder mBreeder;
    QDateTime mStartTime;
    QString mLastSavedDNA;
    QString mLastSavedSVG;
    QString mSaveDirectory;

    void saveAppSettings(void);
    void restoreAppSettings(void);
    void startBreeding(void);
    void stopBreeding(void);
    void loadOriginalImage(const QString& filename);
    void loadDNA(const QString& filename);

    QTimer mAutoSaveTimer;

private slots:
    void loadSVG(const QString& filename);
    void evolved(const QImage&, const DNA&, unsigned int fitness, unsigned int selected, unsigned int generation);
    void proceeded(unsigned int);
    void startStop(void);
    void saveDNA(void);
    void saveSVG(void);
    void openDNA(void);
    void openSVG(void);
    void openOriginalImage(void);
    void resetBreeder(void);
    void about(void);
    void aboutQt(void);
    void imageDropped(const QImage& image, const QString& filename);
    void autoSaveGeneratedImage(void);
};

#endif // __MAINWINDOW_H_

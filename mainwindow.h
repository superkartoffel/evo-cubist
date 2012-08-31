// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QDateTime>
#include <QString>

#include "imagewidget.h"
#include "generationwidget.h"
#include "breeder.h"

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
    ImageWidget* mImageWidget;
    GenerationWidget* mGenerationWidget;
    Breeder mBreeder;
    QDateTime mStartTime;

    void saveAppSettings(void);
    void restoreAppSettings(void);
    void startBreeding(void);
    void stopBreeding(void);
    void loadOriginalImage(const QString&);

private slots:
    void evolved(void);
    void proceeded(void);
    void startStop(void);
    void saveDNA(void);
    void openOriginalImage(void);
};

#endif // __MAINWINDOW_H_

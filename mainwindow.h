// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QTimerEvent>
#include <QFile>
#include <QEvent>
#include <QAction>
#include <QtCore/QDebug>

#include "svgviewer.h"
#include "imagewidget.h"
#include "generationwidget.h"
#include "breeder.h"
#include "optionsform.h"
#include "logviewerform.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

protected:
    bool event(QEvent*);
    void timerEvent(QTimerEvent*);
    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow *ui;
    OptionsForm* mOptionsForm;
    LogViewerForm* mLogViewerForm;
    ImageWidget* mImageWidget;
    GenerationWidget* mGenerationWidget;
    SVGViewer* mSVGViewer;
    Breeder mBreeder;
    QDateTime mStartTime;
    QFile mLog;
    int mAutoStopTimerId;

    unsigned long mRecentEvolvedGeneration;
    unsigned long mRecentEvolvedSelection;

    static const int MaxRecentFiles = 10;
    QAction* mRecentImageFileActs[MaxRecentFiles];
    QAction* mRecentDNAFileActs[MaxRecentFiles];
    QAction* mRecentSettingsFileActs[MaxRecentFiles];

    void saveAppSettings(void);
    void restoreAppSettings(void);
    void startBreeding(void);
    void stopBreeding(void);
    void loadOriginalImage(const QString& filename);
    quint64 totalSeconds(void) const;
    void appendToRecentFileList(const QString& fileName, const QString& listName);
    QString mostRecentFileInList(const QString&);
    void doLog(unsigned long generation, unsigned long selected, int numPoints, int numgenes, quint64 fitness, const QImage& image);
    void doLog(const QString& message);
    void evolved(void);
    bool autoSaveImage(void);
    bool autoSaveDNA(void);

    QTimer mAutoSaveTimer;

private slots:
    bool loadSettings(const QString& filename);
    void loadDNA(const QString& filename);
    void evolved(const QImage&, const DNA&, quint64 fitness, unsigned long selected, unsigned long generation);
    void imageDropped(const QImage&);
    void proceeded(unsigned long generation);
    void startStop(void);
    void openSettings(void);
    void saveSettings(void);
    void saveDNA(void);
    void openDNA(void);
    void openOriginalImage(void);
    void resetBreeder(void);
    void about(void);
    void aboutQt(void);
    void autoSave(void);
    void autoSaveIntervalChanged(int);
    void autoSaveToggled(bool);
    void loadRecentImageFile(void);
    void loadRecentDNAFile(void);
    void loadRecentSettingsFile(void);
    void updateRecentFileActions(const QString& listName, QMenu* menu, QAction* actions[]);
    void copyPicture(int generation, int selected);
    void showPicture(int generation, int selected);
    void gotoPicture(int generation, int selected);
    void startDistributionChanged(void);
    void setBackgroundColor(QRgb);
    void setDeltaR(int);
    void setDeltaG(int);
    void setDeltaB(int);
    void setDeltaA(int);
    void setDeltaXY(int);
};

#endif // __MAINWINDOW_H_

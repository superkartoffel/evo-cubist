// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __LOGVIEWERFORM_H_
#define __LOGVIEWERFORM_H_

#include <QWidget>
#include <QShowEvent>
#include <QString>
#include <QMenu>
#include <QPoint>


namespace Ui {
class LogViewerForm;
}


class LogViewerForm : public QWidget
{
    Q_OBJECT
    
public:
    LogViewerForm(QWidget* parent = NULL);
    ~LogViewerForm();
    void log(unsigned long generation, unsigned long selected, int numPoints, int numGenes, quint64 fitness, const QImage& image);
    void highlightLastRow(void);

protected:
    void showEvent(QShowEvent*);

private:
    Ui::LogViewerForm *ui;
    QMenu* mMenu;
    enum Commands { ShowPicture, CopyToClipboard, GoToFolder };
    static const QColor HighlightColor;

private slots:
    void clear(void);
    void provideContextMenu(const QPoint&);
    void autofitTableContents(void);

signals:
    void showPicture(int generation, int selected);
    void copyPicture(int generation, int selected);
    void gotoPicture(int generation, int selected);

};

#endif // __LOGVIEWERFORM_H_

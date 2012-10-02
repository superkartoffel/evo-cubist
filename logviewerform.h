// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#ifndef __LOGVIEWERFORM_H_
#define __LOGVIEWERFORM_H_

#include <QWidget>
#include <QShowEvent>
#include <QString>

namespace Ui {
class LogViewerForm;
}

class LogViewerForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit LogViewerForm(QWidget* parent = NULL);
    ~LogViewerForm();
    void log(unsigned long generation, unsigned long selected, int numPoints, int numGenes, quint64 fitness, const QImage& image);

protected:
    void showEvent(QShowEvent*);

private:
    Ui::LogViewerForm *ui;

private slots:
    void cellDoubleClicked(int, int);
};

#endif // __LOGVIEWERFORM_H_

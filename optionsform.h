#ifndef OPTIONSFORM_H
#define OPTIONSFORM_H

#include <QWidget>

namespace Ui {
class OptionsForm;
}

class OptionsForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit OptionsForm(QWidget *parent = 0);
    ~OptionsForm();
    
private:
    Ui::OptionsForm *ui;
};

#endif // OPTIONSFORM_H

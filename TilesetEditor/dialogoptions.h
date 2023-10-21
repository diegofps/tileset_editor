#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include "ui_dialogoptions.h"
#include <QDialog>

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent = nullptr);
    ~DialogOptions();
    void setOptions(QStringList options, int defaultSelection=0);
    int selectedOption() const;

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::DialogOptions * ui;

};

#endif // DIALOGOPTIONS_H

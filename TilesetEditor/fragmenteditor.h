#ifndef FRAGMENTEDITOR_H
#define FRAGMENTEDITOR_H

#include "appstate.h"
#include <QPushButton>
#include <QWidget>

namespace Ui {
class FragmentEditor;
}

class FragmentEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentEditor(QWidget *parent = nullptr);
    ~FragmentEditor();

private:
    void updateWidgetEditor();

private:
    Ui::FragmentEditor *ui;
};

#endif // FRAGMENTEDITOR_H

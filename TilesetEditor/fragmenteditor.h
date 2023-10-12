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
    void styleToolButtons(const EditorTool value);
    void styleButton(bool const value, QPushButton * const button);

private:
    Ui::FragmentEditor *ui;
};

#endif // FRAGMENTEDITOR_H

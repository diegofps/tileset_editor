#ifndef FRAGMENTEDITOR_H
#define FRAGMENTEDITOR_H

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
    Ui::FragmentEditor *ui;
};

#endif // FRAGMENTEDITOR_H

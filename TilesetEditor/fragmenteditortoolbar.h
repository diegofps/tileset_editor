#ifndef FRAGMENTEDITORTOOLBAR_H
#define FRAGMENTEDITORTOOLBAR_H

#include <QWidget>

namespace Ui {
class FragmentEditorToolbar;
}

class FragmentEditorToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentEditorToolbar(QWidget *parent = nullptr);
    ~FragmentEditorToolbar();

private:
    Ui::FragmentEditorToolbar *ui;
};

#endif // FRAGMENTEDITORTOOLBAR_H

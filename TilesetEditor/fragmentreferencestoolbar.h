#ifndef FRAGMENTREFERENCESTOOLBAR_H
#define FRAGMENTREFERENCESTOOLBAR_H

#include <QWidget>

namespace Ui {
class FragmentReferencesToolbar;
}

class FragmentReferencesToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentReferencesToolbar(QWidget *parent = nullptr);
    ~FragmentReferencesToolbar();

private:
    Ui::FragmentReferencesToolbar *ui;
};

#endif // FRAGMENTREFERENCESTOOLBAR_H

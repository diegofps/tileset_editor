#ifndef FRAGMENTREFERENCES_H
#define FRAGMENTREFERENCES_H

#include "appstate.h"
#include <QWidget>

namespace Ui {
class FragmentReferences;
}

class FragmentReferences : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentReferences(QWidget *parent = nullptr);
    ~FragmentReferences();

private:
    void styleScreenshotButtons(ReferenceMode const value);
    void updateReferenceWidget();

private:
    Ui::FragmentReferences *ui;
};

#endif // FRAGMENTREFERENCES_H

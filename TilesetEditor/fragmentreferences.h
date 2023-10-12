#ifndef FRAGMENTREFERENCES_H
#define FRAGMENTREFERENCES_H

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
    void styleScreenshotButtons(const int value);
    void styleHighlightPositionButton(const bool value);

private:
    Ui::FragmentReferences *ui;
};

#endif // FRAGMENTREFERENCES_H

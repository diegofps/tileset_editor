#ifndef FRAGMENTPALETTES_H
#define FRAGMENTPALETTES_H

#include <QWidget>

namespace Ui {
class FragmentPalettes;
}

class FragmentPalettes : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentPalettes(QWidget *parent = nullptr);
    ~FragmentPalettes();

private:
    Ui::FragmentPalettes *ui;
};

#endif // FRAGMENTPALETTES_H

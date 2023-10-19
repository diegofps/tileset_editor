#ifndef FRAGMENTTILES_H
#define FRAGMENTTILES_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class FragmentTiles;
}

class FragmentTiles : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentTiles(QWidget *parent = nullptr);
    ~FragmentTiles();

private:
    Ui::FragmentTiles *ui;
    void styleButton(QPushButton * btn, bool value);
};

#endif // FRAGMENTTILES_H

#ifndef FRAGMENTTILES_H
#define FRAGMENTTILES_H

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
    void styleButtons(const QString &value);
};

#endif // FRAGMENTTILES_H

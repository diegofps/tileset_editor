#ifndef FRAGMENTTILESETPROPERTIES_H
#define FRAGMENTTILESETPROPERTIES_H

#include <QWidget>

namespace Ui {
class FragmentTilesetProperties;
}

class FragmentTilesetProperties : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentTilesetProperties(QWidget *parent = nullptr);
    ~FragmentTilesetProperties();

private:
    Ui::FragmentTilesetProperties *ui;
};

#endif // FRAGMENTTILESETPROPERTIES_H

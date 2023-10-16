#ifndef FRAGMENTTILESETPROPERTIES_H
#define FRAGMENTTILESETPROPERTIES_H

#include "model/tileset.h"
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
    void loadTileset(Tileset *ts);

private:
    Ui::FragmentTilesetProperties *ui;
};

#endif // FRAGMENTTILESETPROPERTIES_H

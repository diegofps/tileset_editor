#ifndef FRAGMENTTILESETS_H
#define FRAGMENTTILESETS_H

#include "model/tileset.h"
#include <QWidget>

namespace Ui {
class FragmentTilesets;
}

class FragmentTilesets : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentTilesets(QWidget *parent = nullptr);
    ~FragmentTilesets();

private:
    void filterTilesets(int sceneID, QList<Tileset*> const * value);
    void updateTilesetsWidget();

private:
    Ui::FragmentTilesets * ui;
    QList<Tileset*> _tilesets;
};

#endif // FRAGMENTTILESETS_H

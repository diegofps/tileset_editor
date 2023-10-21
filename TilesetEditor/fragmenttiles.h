#ifndef FRAGMENTTILES_H
#define FRAGMENTTILES_H

#include "model/tile.h"
#include "appstate.h"
#include "widgetgridtiles.h"
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
    void loadTiles(int sceneID, QList<Tile *> const * tiles, TilesFilter * filter);
    void styleButton(QPushButton * btn, int value);

private:
    Ui::FragmentTiles *ui;
    QList<Tile*> _tiles;
    WidgetGridTiles * _gridTiles;
};

#endif // FRAGMENTTILES_H

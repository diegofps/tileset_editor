#ifndef FRAGMENTTILES_H
#define FRAGMENTTILES_H

#include "model/tile.h"
#include "appstate.h"
#include "widgettiles.h"
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
    void filterTiles(int sceneID, const QList<Tile *> *tiles, TilesFilter *filter);
    void updateTilesWidget();
    void styleButton(QPushButton * btn, int value);
    void restoreSelectedTile();
    void saveSelectedTile();

private:
    Ui::FragmentTiles *ui;
    QList<Tile*> _tiles;
    WidgetTiles * _widgetTiles;
    int _lastSelectedItemID;
};

#endif // FRAGMENTTILES_H

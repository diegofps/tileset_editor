#ifndef TILECACHE_H
#define TILECACHE_H

#include "model/palette.h"
#include "model/tile.h"

#include <QPixmap>

class TileCache
{
public:
    TileCache();
    void clear();
    QPixmap * getTilePixmap(Tile * tile, Palette * palette);

private:
    QHash<QPair<int,int>,QPixmap*> _cache;

};

#endif // TILECACHE_H

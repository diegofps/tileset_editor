#ifndef TILECACHEHD_H
#define TILECACHEHD_H

#include "model/palette.h"
#include "model/tile.h"
#include <QPixmap>



class TileCacheHD
{
public:
    TileCacheHD();
    void clear();
    QPixmap * getTilePixmap(Tile *tile, Palette *palette);

private:
    QHash<QPair<int,int>,QPixmap*> _cache;
};

#endif // TILECACHEHD_H

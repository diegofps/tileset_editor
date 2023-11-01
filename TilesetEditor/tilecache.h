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

    QPixmap * getMaskPixmap(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip);
    QImage *getMaskImage(Tile *tile, Palette *palette, const bool hFlip, const bool vFlip);

    QPixmap * getTilePixmap(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip);
    QImage *getTileImage(Tile *tile, Palette *palette, const bool hFlip, const bool vFlip);

    void loadImage(Tile *tile, Palette *palette, bool hFlip, bool vFlip, QImage &img);
    void loadMask(Tile *tile, bool hFlip, bool vFlip, QImage &img);

private:
    void createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                     Tile *tile, Palette *palette, bool hFlip, bool vFlip);

private:
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedMaskPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedMaskImages;

    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedImages;

};

#endif // TILECACHE_H

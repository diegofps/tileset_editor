#ifndef TILECACHEHD_H
#define TILECACHEHD_H

#include "model/palette.h"
#include "model/tile.h"
#include <QPixmap>

struct TileFile
{
    QString filename;
    int tileID;
    int paletteID;
    int tilesetID;
    bool hFlip;
    bool vFlip;
};

class TileCacheHD
{
public:

    TileCacheHD();
    void clear();
    QPixmap * getTilePixmap(Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    void createCache(QPair<QPair<int, int>, QPair<bool, bool> > &key, Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    QImage * loadImage(Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    void updateKnownTiles(QString const & projectPath);

private:

    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedImages;
    QHash<int,TileFile*> _tileID2tileFile;

};

#endif // TILECACHEHD_H

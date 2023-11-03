#ifndef TILECACHEHD_H
#define TILECACHEHD_H

#include "model/palette.h"
#include "model/tile.h"
#include <QPixmap>

inline int encodeHDPixel(int const hdValue, int const original)
{
    if (hdValue>original)
        return (hdValue-original-1)*127/(255-original-1)+1;
    else if (original==0)
        return 0;
    else
        return -(hdValue*128/original);
}

inline int decodeHDPixel(int const encodedValue, int const original)
{
    if (encodedValue<=0)
        return (-encodedValue)*(original)/128;
    else
        return original+1+(encodedValue-1)*(255-original-1)/127;
}

class TileCacheHD
{
public:

    TileCacheHD();
    void clear();
    QPixmap * getTilePixmap(Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    void createCache(QPair<QPair<int, int>, QPair<bool, bool> > &key, Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    void loadImage(Tile *tile, Palette *palette, bool hFlip, bool vFlip, QImage &img);

private:

//    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedMaskPixmaps;
//    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedMaskImages;

    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedImages;

};

#endif // TILECACHEHD_H

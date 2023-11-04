#ifndef TILECACHEHD_H
#define TILECACHEHD_H

#include "model/palette.h"
#include "model/tile.h"
#include <QPixmap>

inline int encodeHDChannel(int const hdValue, int const original)
{
    if (hdValue>original)
        return (hdValue-original-1)*127/(255-original-1)+1;
    else if (original==0)
        return 0;
    else
        return -(hdValue*128/original);
}

inline int decodeHDChannel(int const encodedValue, int const original)
{
    if (encodedValue<=0)
        return (-encodedValue)*(original)/128;
    else
        return original+1+(encodedValue-1)*(255-original-1)/127;
}

inline void encodeHDColor(uchar * const data, QRgb const hdImgColor, QRgb const refColor)
{
    int refH, refS, refL, refA;
    int hdH, hdS, hdL, hdA;

    QColor::fromRgba(refColor).getHsl(&refH, &refS, &refL, &refA);
    QColor::fromRgba(hdImgColor).getHsl(&hdH, &hdS, &hdL, &hdA);

    int offH;

    if (hdH < refH)
        offH = 360 + hdH - refH;
    else
        offH = hdH - refH;

    data[0] = (char) 255;
    data[1] = offH * 255 / 360;
    data[2] = encodeHDChannel(hdS, refS);
    data[3] = encodeHDChannel(hdL, refL);
}

inline QRgb decodeHDColor(uchar * const data, QRgb const refColor)
{
    int refH, refS, refL;
    QColor::fromRgba(refColor).getHsl(&refH, &refS, &refL);

    int const offH = data[1];
    int const offS = (char)data[2];
    int const offL = (char)data[3];

    int hdH = refH + offH * 360 / 255;
    int hdS = decodeHDChannel(offS, refS);
    int hdL = decodeHDChannel(offL, refL);

    if (hdH >= 360) hdH -= 360;

    return QColor::fromHsl(hdH, hdS, hdL).rgb();
}

#define MERGE_COLORS(c1,c2,f) qRgba(\
    qRed(c1)*(1-f)+qRed(c2)*f,\
    qGreen(c1)*(1-f)+qGreen(c2)*f,\
    qBlue(c1)*(1-f)+qBlue(c2)*f,\
    qAlpha(c1)*(1-f)+qAlpha(c2)*f)

class TileCacheHD
{
public:

    TileCacheHD();
    void clear();
    QPixmap * getTilePixmap(Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    void createCache(QPair<QPair<int, int>, QPair<bool, bool> > &key, Tile *tile, Palette *palette, bool hFlip, bool vFlip);
    QImage * loadImage(Tile *tile, Palette *palette, bool hFlip, bool vFlip);

private:

//    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedMaskPixmaps;
//    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedMaskImages;

    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedImages;

};

#endif // TILECACHEHD_H

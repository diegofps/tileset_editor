#ifndef TILECACHEHD_H
#define TILECACHEHD_H

#include "model/palette.h"
#include "model/tile.h"
#include <QPixmap>

inline int encodeHDChannel(int const hdValue, int const original)
{
    int result;

    if (hdValue>original)
        result = (hdValue-original)*127/(255-original)+1;

    else if (original==0)
        result = 0;

    else
        result = -(hdValue*128/original);

    if (result < -128 || result > 128)
        qWarning() << "Encode error";

    return result;
}

inline int decodeHDChannel(int const encodedValue, int const original)
{
    int result;

    if (encodedValue<=0)
        result = (-encodedValue)*(original)/128;
    else
        result = original+(encodedValue-1)*(255-original)/127;

    if (result < 0 || result > 255)
        qWarning() << "Decode error";

    return result;
}

inline void encodeHDColor(uchar * const data, QRgb const hdImgColor, QRgb const refColor)
{
    int refH, refS, refV, refA;
    int hdH, hdS, hdV, hdA;

    QColor::fromRgba(refColor).getHsv(&refH, &refS, &refV, &refA);
    QColor::fromRgba(hdImgColor).getHsv(&hdH, &hdS, &hdV, &hdA);

    int offH;

    if (hdH < refH)
        offH = 360 + hdH - refH;
    else
        offH = hdH - refH;

    data[0] = (char) 255;
    data[1] = offH * 255 / 360;
    data[2] = encodeHDChannel(hdS, refS);
    data[3] = encodeHDChannel(hdV, refV);
}

inline QRgb decodeHDColor(uchar * const data, QRgb const refColor)
{
    int refH, refS, refV;
    QColor::fromRgba(refColor).getHsv(&refH, &refS, &refV);

    int const offH = data[1];
    int const offS = (char)data[2];
    int const offL = (char)data[3];

//    if (refS == 255 && data[2] == 15)
//        qWarning("Found it 1!");

    int hdH = refH + offH * 360 / 255;
    int hdS = decodeHDChannel(offS, refS);
    int hdV = decodeHDChannel(offL, refV);

    if (hdH >= 360) hdH -= 360;

    if (hdH >= 360 || hdH < -1 || hdS < 0 || hdS > 255 || hdV < 0 || hdV > 255)
        qWarning() << "Found it 2!";

    return QColor::fromHsv(hdH, hdS, hdV).rgb();
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

    void parseTiles();

private:

    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QPixmap*> _cachedPixmaps;
    QHash<QPair<QPair<int,int>,QPair<bool,bool>>,QImage*> _cachedImages;

};

#endif // TILECACHEHD_H

#ifndef TILE_H
#define TILE_H

#include <QJsonObject>
#include <unordered_map>

class Tile
{
private:

    QByteArray _uniqueKey;

public:

    int id;
    QHash<int, int> palettesUsed; // Memorizes the palette colors and frequency they were used
    int pixels[64];      // The pixel colors before conversion to RGB, represented as color palette indexes (pseudocolors).
    int paletteSize;     // The number of colors it uses in the palette. May be 1<<2, 1<<4, or 1<<8.
    int seenOnFrames;    // Number of different frames it was seen, used to capture reference screenshots;
//    Reference references[3];

public:

    Tile(QJsonObject & data);
    QJsonObject exportAsJson();
    QByteArray & uniqueKey();
//    void import(Tile * other);

};

#endif // TILE_H

#ifndef TILE_H
#define TILE_H

#include <QJsonObject>
#include <unordered_map>

class Tile
{
private:

    QByteArray _uniqueKey;

public:

    int  id;
    int  sceneId;
    int  pixels[64];      // The pixel colors before conversion to RGB, represented as color palette indexes (pseudocolors).
    int  paletteSize;     // The number of colors it uses in the palette. May be 1<<2, 1<<4, or 1<<8.
    int  seenOnFrames;    // Number of different frames it was seen, used to capture reference screenshots;
    bool usedInSprite;
    bool usedInBackground;
    int  ref1ID;
    int  ref25ID;
    int  ref50ID;
    int  ref75ID;
    int  ref100ID;
    int  refNNID;
    int  refFNID;
    int  refNFID;
    int  refFFID;
    int  preferredPalette; // The palette id most used by this tile

    QHash<int, int> palettesUsed; // Memorizes the palette colors and frequency they were used

public:

    Tile(QJsonObject & data);
    QJsonObject exportAsJson();
    QByteArray & uniqueKey();
    bool usedWithHFlip();
    bool usedWithVFlip();

};

#endif // TILE_H

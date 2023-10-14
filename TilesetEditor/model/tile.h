#ifndef TILE_H
#define TILE_H

#include <QJsonObject>
#include <unordered_map>
#include "refscreenshot.h"

class Tile
{
private:

    std::unordered_map<int, int> _palettesUsed; // Memorizes the palette colors and frequency they were used
    int _pixels[64];      // The pixel colors before conversion to RGB, represented as color palette indexes (pseudocolors).
    int _paletteSize;     // The number of colors it uses in the palette. May be 1<<2, 1<<4, or 1<<8.
    int _lastSeenOnFrame; // Number from IPPU.TotalEmulatedFrames
    int _seenOnFrames;    // Number of different frames it was seen, used to capture reference screenshots;
    RefScreenshot _refScreenshots[3];

public:

    Tile(QJsonObject data);

};

#endif // TILE_H

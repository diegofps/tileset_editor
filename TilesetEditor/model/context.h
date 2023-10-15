#ifndef CONTEXT_H
#define CONTEXT_H

#include <QJsonObject>

class Context
{
public:

    int lastTileID;
    int lastPaletteID;
    int lastTilesetID;
    int lastReferenceID;
    int lastScreenshotID;

    Context();

    void initFromJson(QJsonObject data);
    QJsonObject exportAsJson();
    void clear();

    // TODO: Save context info like state IDs (for tiles, tilesets, palettes, and screenshots) and splitter positions.
};

#endif // CONTEXT_H

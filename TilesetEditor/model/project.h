#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonObject>

class Project
{
public:

    int lastTileID;
    int lastPaletteID;
    int lastTilesetID;
    int lastReferenceID;
    int lastScreenshotID;

    Project();

    bool initFromJson(QJsonObject data);
    QJsonObject exportAsJson();
    void clear();

    // TODO: Save context info like state IDs (for tiles, tilesets, palettes, and screenshots) and splitter positions.
};

#endif // PROJECT_H

#include "context.h"

Context::Context()
{
    clear();
}

void Context::initFromJson(QJsonObject data)
{
    lastTileID = data["lastTileID"].toInt();
    lastPaletteID = data["lastPaletteID"].toInt();
    lastTilesetID = data["lastTilesetID"].toInt();
    lastReferenceID = data["lastReferenceID"].toInt();
    lastScreenshotID = data["lastScreenshotID"].toInt();
}

QJsonObject Context::exportAsJson()
{
    QJsonObject data;
    data["lastTileID"] = lastTileID;
    data["lastPaletteID"] = lastPaletteID;
    data["lastTilesetID"] = lastTilesetID;
    data["lastReferenceID"] = lastReferenceID;
    data["lastScreenshotID"] = lastScreenshotID;
    return data;
}

void Context::clear()
{
    lastTileID = 0;
    lastPaletteID = 0;
    lastTilesetID = 0;
    lastReferenceID = 0;
    lastScreenshotID = 0;
}

//Context::Context(QJsonObject data)
//{

//}

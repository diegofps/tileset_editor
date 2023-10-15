#include "project.h"

Project::Project()
{
    clear();
}

bool Project::initFromJson(QJsonObject data)
{
    lastTileID = data["lastTileID"].toInt();
    lastPaletteID = data["lastPaletteID"].toInt();
    lastTilesetID = data["lastTilesetID"].toInt();
    lastReferenceID = data["lastReferenceID"].toInt();
    lastScreenshotID = data["lastScreenshotID"].toInt();
    return true;
}

QJsonObject Project::exportAsJson()
{
    QJsonObject data;
    data["lastTileID"] = lastTileID;
    data["lastPaletteID"] = lastPaletteID;
    data["lastTilesetID"] = lastTilesetID;
    data["lastReferenceID"] = lastReferenceID;
    data["lastScreenshotID"] = lastScreenshotID;
    return data;
}

void Project::clear()
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

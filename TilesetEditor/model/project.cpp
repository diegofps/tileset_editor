#include "project.h"

Project::Project()
{
    clear();
}

bool Project::initFromJson(QJsonObject data)
{
    dbVersion = data["DBVersion"].toInt(1);
    lastTileID = data["lastTileID"].toInt();
    lastPaletteID = data["lastPaletteID"].toInt();
    lastTilesetID = data["lastTilesetID"].toInt();
    lastReferenceID = data["lastReferenceID"].toInt();
    lastScreenshotID = data["lastScreenshotID"].toInt();
    lastClusterID = data["lastClusterID"].toInt();
    return true;
}

QJsonObject Project::exportAsJson()
{
    QJsonObject data;
    data["DBVersion"] = dbVersion;
    data["lastTileID"] = lastTileID;
    data["lastPaletteID"] = lastPaletteID;
    data["lastTilesetID"] = lastTilesetID;
    data["lastReferenceID"] = lastReferenceID;
    data["lastScreenshotID"] = lastScreenshotID;
    data["lastClusterID"] = lastClusterID;
    return data;
}

void Project::clear()
{
    dbVersion = 1;
    lastTileID = 0;
    lastPaletteID = 0;
    lastTilesetID = 0;
    lastReferenceID = 0;
    lastScreenshotID = 0;
    lastClusterID = 0;
}

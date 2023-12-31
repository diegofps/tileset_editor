#include "project.h"

#include "helpers/json.h"
Project::Project()
{
    clear();
}

bool Project::initFromJson(QJsonObject data)
{
    getIntOrFail(dbVersion, data, "Project", "DBVersion");
    getIntOrFail(lastTileID, data, "Project", "LastTileID");
    getIntOrFail(lastPaletteID, data, "Project", "LastPaletteID");
    getIntOrFail(lastTilesetID, data, "Project", "LastTilesetID");
    getIntOrFail(lastReferenceID, data, "Project", "LastReferenceID");
    getIntOrFail(lastScreenshotID, data, "Project", "LastScreenshotID");
    getIntOrFail(lastSceneID, data, "Project", "LastSceneID");
    getIntOrFail(lastCellID, data, "Project", "LastCellID");

    return true;
}

QJsonObject Project::exportAsJson()
{
    QJsonObject data;
    data["DBVersion"] = dbVersion;
    data["LastTileID"] = lastTileID;
    data["LastPaletteID"] = lastPaletteID;
    data["LastTilesetID"] = lastTilesetID;
    data["LastReferenceID"] = lastReferenceID;
    data["LastScreenshotID"] = lastScreenshotID;
    data["LastSceneID"] = lastSceneID;
    data["LastCellID"] = lastCellID;
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
    lastSceneID = 0;
    lastCellID = 0;
}


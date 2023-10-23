#include "tileset.h"
#include "errors.h"

#include <QJsonArray>

#include "jsonhelpers.h"

Tileset::Tileset()
{

}

Tileset::Tileset(QJsonObject data)
{
    getIntOrFail(id, data, "Tileset", "ID");
    getIntOrDefault(sceneId, data, "Tileset", "SceneID", 0);
    getQStringOrFail(name, data, "Tileset", "Name");
    getIntOrFail(gridW, data, "Tileset", "GridW");
    getIntOrFail(gridH, data, "Tileset", "GridH");
    getQColorOrFail(bgColor, data, "Tileset", "Color");
}

QJsonObject Tileset::exportAsJson()
{
    QJsonArray jColor;
    jColor.append(bgColor.red());
    jColor.append(bgColor.green());
    jColor.append(bgColor.blue());

    QJsonObject data;
    data["ID"] = id;
    data["SceneID"] = sceneId;
    data["Name"] = name;
    data["GridW"] = gridW;
    data["GridH"] = gridH;
    data["Color"] = jColor;

    return data;
}

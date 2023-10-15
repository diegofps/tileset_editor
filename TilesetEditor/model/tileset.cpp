#include "tileset.h"
#include "errors.h"

#include <QJsonArray>

Tileset::Tileset()
{

}

Tileset::Tileset(QJsonObject data)
{
    // ID

    if (!data.contains("ID"))
        throw ContextError("Tileset is missing attribute 'ID'");

    if (!data["ID"].isDouble())
        throw ContextError("Tileset has the wrong value type associated to 'ID'");

    id = data["ID"].toInt();

    // Name

    if (!data.contains("Name"))
        throw ContextError("Tileset is missing attribute 'Name'");

    if (!data["Name"].isString())
        throw ContextError("Tileset has the wrong value type associated to 'Name'");

    name = data["Name"].toString();

    // GridW

    if (!data.contains("GridW"))
        throw ContextError("Tileset is missing attribute 'GridW'");

    if (!data["GridW"].isDouble())
        throw ContextError("Tileset has the wrong value type associated to 'GridW'");

    gridW = data["GridW"].toInt();

    // GridH

    if (!data.contains("GridH"))
        throw ContextError("Tileset is missing attribute 'GridH'");

    if (!data["GridH"].isDouble())
        throw ContextError("Tileset has the wrong value type associated to 'GridH'");

    gridH = data["GridH"].toInt();

    // Color

    if (!data.contains("Color"))
        throw ContextError("Tileset is missing attribute 'Color'");

    if (!data["Color"].isArray())
        throw ContextError("Tileset has the wrong value type associated to 'Color'");

    QJsonArray jColor = data["Color"].toArray();

    if (jColor.size() != 3)
        throw ContextError("Tileset has the wrong size for array associated to 'Color'");

    if (!jColor[0].isDouble() || !jColor[1].isDouble() || !jColor[2].isDouble())
        throw ContextError("Tileset has the wrong value type inside 'Color'");

    int const r = jColor[0].toInt();
    int const g = jColor[1].toInt();
    int const b = jColor[2].toInt();

    bgColor = QColor::fromRgb(r,g,b);
}

QJsonObject Tileset::exportAsJson()
{
    QJsonArray jColor;
    jColor.append(bgColor.red());
    jColor.append(bgColor.green());
    jColor.append(bgColor.blue());

    QJsonObject data;
    data["ID"] = id;
    data["Name"] = name;
    data["GridW"] = gridW;
    data["GridH"] = gridH;
    data["Color"] = jColor;

    return data;
}

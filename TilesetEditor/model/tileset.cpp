#include "tileset.h"
#include "errors.h"
#include "jsonhelpers.h"

#include <QJsonArray>

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

    if (data.contains("Cells"))
    {
        if (!data["Cells"].isArray())
            throw ContextError("Tileset expects an array in attribute 'Cells'");

        QJsonArray jCells = data["Cells"].toArray();

        for (qsizetype i=0;i!=jCells.size();++i)
        {
            if (!jCells[i].isObject())
                throw ContextError(QString("Tileset expects an object in attribute 'Cells', position %1").arg(i));

            auto jCell = jCells[i].toObject();
            Cell * cell = new Cell(jCell);
            cells[QPair<int,int>(cell->x, cell->y)] = cell;
        }
    }
}

Tileset::~Tileset()
{
    for (auto cell : cells.asKeyValueRange())
        delete cell.second;
}

QJsonObject Tileset::exportAsJson()
{
    QJsonArray jCells;
    for (auto cell : cells.asKeyValueRange())
        jCells.append(cell.second->exportAsJson());

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
    data["Cells"] = jCells;

    return data;
}

#include "tileset.h"
#include "errors.h"
#include "jsonhelpers.h"
#include "editorcommand.h"

#include <QJsonArray>

Tileset::Tileset() :
    _historyPosition(0)
{

}

Tileset::Tileset(QJsonObject data) :
    _historyPosition(0)
{
    getIntOrFail(id, data, "Tileset", "ID");
    getIntOrDefault(sceneId, data, "Tileset", "SceneID", 0);
    getIntOrFail(gridW, data, "Tileset", "GridW");
    getIntOrFail(gridH, data, "Tileset", "GridH");
    getQStringOrFail(name, data, "Tileset", "Name");
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
    data["GridW"] = gridW;
    data["GridH"] = gridH;
    data["Name"] = name;
    data["Color"] = jColor;
    data["Cells"] = jCells;

    return data;
}

void Tileset::historyClear()
{
    for (auto cmd : _history)
        delete cmd;

    _history.clear();
    _historyPosition = 0;
}

void Tileset::historyAdd(AppState * state, CellCommand * cmd)
{
    for (qsizetype tmp=_historyPosition;tmp<_history.size();++tmp)
        delete _history[tmp];

    _history.resize(_historyPosition+1);
    _history[_historyPosition++] = cmd;
    cmd->execute(state);
}

bool Tileset::historyUndo(AppState * state)
{
    if (_historyPosition == 0)
        return false;

    _history[--_historyPosition]->unexecute(state);
    return true;
}

bool Tileset::historyRedo(AppState * state)
{
    if (_historyPosition == _history.size())
        return false;

    _history[_historyPosition++]->execute(state);
    return true;
}

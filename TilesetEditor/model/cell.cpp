#include "cell.h"
#include "helpers/json.h"

Cell::Cell() :
    id(0),
    tileID(0),
    paletteID(0),
    x(0),
    y(0),
    hFlip(false),
    vFlip(false)
{

}

Cell::Cell(QJsonObject & data)
{
    getIntOrFail(id, data, "Cell", "ID");
    getIntOrFail(tileID, data, "Cell", "TileID");
    getIntOrFail(paletteID, data, "Cell", "PaletteID");
    getIntOrFail(x, data, "Cell", "X");
    getIntOrFail(y, data, "Cell", "Y");
    getBoolOrFail(hFlip, data, "Cell", "HFlip");
    getBoolOrFail(vFlip, data, "Cell", "VFlip");
}

QJsonObject Cell::exportAsJson()
{
    QJsonObject data;
    data["ID"] = id;
    data["TileID"] = tileID;
    data["PaletteID"] = paletteID;
    data["X"] = x;
    data["Y"] = y;
    data["HFlip"] = hFlip;
    data["VFlip"] = vFlip;
    return data;
}

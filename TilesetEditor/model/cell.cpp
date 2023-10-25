#include "cell.h"
#include "jsonhelpers.h"

Cell::Cell(QJsonObject & data)
{
    getIntOrFail(id, data, "Cell", "ID");
    getIntOrFail(tileID, data, "Cell", "TileID");
    getIntOrFail(paletteID, data, "Cell", "PaletteID");
    getIntOrFail(x, data, "Cell", "X");
    getIntOrFail(y, data, "Cell", "Y");
    getBoolOrFail(hFlip, data, "Cell", "HFlip");
    getBoolOrFail(vFlip, data, "Cell", "VFlip");
    getBoolOrFail(isLink, data, "Cell", "IsLink");
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
    data["IsLink"] = isLink;
    return data;
}

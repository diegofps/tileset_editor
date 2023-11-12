#include "reference.h"
#include "helpers/json.h"

Reference::Reference(QJsonObject & data)
{
    getIntOrFail(id, data, "Reference", "ID");
    getIntegerOrFail(tile, data, "Reference", "Tile");
    getIntOrFail(screenshotId, data, "Reference", "ScreenshotID");
    getIntOrFail(frame, data, "Reference", "Frame");
    getIntOrFail(x, data, "Reference", "X");
    getIntOrFail(y, data, "Reference", "Y");
    getIntOrFail(startLine, data, "Reference", "StartLine");
    getIntOrFail(lineCount, data, "Reference", "LineCount");
    getIntOrFail(colorPaletteID, data, "Reference", "ColorPaletteID");
    getBoolOrFail(hFlip, data, "Reference", "HFlip");
    getBoolOrFail(vFlip, data, "Reference", "VFlip");
    getQStringOrFail(MATH, data, "Reference", "MATH");
    getQStringOrFail(PIXEL, data, "Reference", "PIXEL");
    getQStringOrFail(OP, data, "Reference", "OP");
    getQStringOrFail(BPSTART, data, "Reference", "BPSTART");
    getQStringOrFail(TILE, data, "Reference", "TILE");
}

QJsonObject Reference::exportAsJson() const
{
    QJsonObject data;
    data["ID"] = id;
    data["Tile"] = (qint64) tile;
    data["ScreenshotID"] = screenshotId;
    data["Frame"] = frame;
    data["X"] = x;
    data["Y"] = y;
    data["StartLine"] = startLine;
    data["LineCount"] = lineCount;
    data["ColorPaletteID"] = colorPaletteID;
    data["HFlip"] = hFlip;
    data["VFlip"] = vFlip;
    data["MATH"] = MATH;
    data["PIXEL"] = PIXEL;
    data["OP"] = OP;
    data["BPSTART"] = BPSTART;
    data["TILE"] = TILE;
    return data;
}

void Reference::clear()
{
    screenshotId = 0;
}

bool Reference::isEmpty() const
{
    return screenshotId == 0;
}

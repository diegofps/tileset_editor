#include "reference.h"

Reference::Reference(QJsonObject & data)
{
    id = data["ID"].toInt();
    tile = data["Tile"].toInteger();
    screenshotId = data["ScreenshotID"].toInt();
    frame = data["Frame"].toInt();
    x = data["X"].toInt();
    y = data["Y"].toInt();
    startLine = data["StartLine"].toInt();
    lineCount = data["LineCount"].toInt();
    colorPaletteID = data["ColorPaletteID"].toInt();
    MATH = data["MATH"].toString();
    PIXEL = data["PIXEL"].toString();
    OP = data["OP"].toString();
    BPSTART = data["BPSTART"].toString();
    TILE = data["TILE"].toString();
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
    data["MATH"] = MATH;
    data["PIXEL"] = PIXEL;
    data["OP"] = OP;
    data["BPSTART"] = BPSTART;
    data["TILE"] = TILE;
    return data;
}

//void Reference::import(Reference const & other,
//                       QHash<int, int> & oldPaletteID2NewPaletteID,
//                       QHash<int, int> & oldReferenceID2NewReferenceID)
//{
//    screenshotId = other.screenshotId;
//    frame = other.frame;
//    x = other.x;
//    y = other.y;
//    startLine = other.startLine;
//    lineCount = other.lineCount;
//    colorPaletteID = oldPaletteID2NewPaletteID[other.colorPaletteID];
//    MATH = other.MATH;
//    PIXEL = other.PIXEL;
//    OP = other.OP;
//    BPSTART = other.BPSTART;
//    TILE = other.TILE;
//}

void Reference::clear()
{
    screenshotId = 0;
}

bool Reference::isEmpty() const
{
    return screenshotId == 0;
}

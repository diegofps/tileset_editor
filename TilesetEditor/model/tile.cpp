#include "tile.h"
#include "errors.h"

#include <QJsonArray>

Tile::Tile(QJsonObject & data)
{
    // ID

    if (!data.contains("ID"))
        throw ContextError("Tile is missing attribute 'ID'");

    if (!data["ID"].isDouble())
        throw ContextError("Tile has the wrong value type associated to 'ID'");

    id = data["ID"].toInt();

    // Pixels

    if (!data.contains("Pixels"))
        throw ContextError("Tile is missing attribute 'Pixels'");

    auto jPixels = data["Pixels"].toArray();
    if (jPixels.size() != 64)
        throw ContextError(QString("Wrong number of values in Pixels attribute. Expected 64, got %1.").arg(jPixels.size()));

    for (int i=0;i!=64;++i)
        pixels[i] = jPixels[i].toInt();

    // Palettes

    if (!data.contains("Palettes"))
        throw ContextError("Tile is missing the attribute 'Palettes'");

    if (!data["Palettes"].isObject())
        throw ContextError("Tile has an invalid value for 'Palettes'");

    auto jPalettes = data["Palettes"].toObject();

    for (QString const & key : jPalettes.keys())
        palettesUsed[key.toInt()] = jPalettes[key].toInt();

    // PaletteSize

    if (!data.contains("PaletteSize"))
        throw ContextError("Tile is missing attribute 'PaletteSize'");

    if (!data["PaletteSize"].isDouble())
        throw ContextError("Tile has the wrong value associated to 'PaletteSize'");

    paletteSize = data["PaletteSize"].toInt();

    // UsedInSprite

    if (!data.contains("UsedInSprite"))
        throw ContextError("Tile is missing attribute 'UsedInSprite'");

    if (!data["UsedInSprite"].isBool())
        throw ContextError("Tile has the wrong value type associated to 'UsedInSprite'");

    usedInSprite = data["UsedInSprite"].toBool();

    // UsedInBackground

    if (!data.contains("UsedInBackground"))
        throw ContextError("Tile is missing attribute 'UsedInBackground'");

    if (!data["UsedInBackground"].isBool())
        throw ContextError("Tile has the wrong value type associated to 'UsedInBackground'");

    usedInBackground = data["UsedInBackground"].toBool();

    // SeenOnFrames

    seenOnFrames = data.contains("SeenOnFrames") ? data["SeenOnFrames"].toInt(-1) : -1;

    // Unique key

    _uniqueKey.clear();

    // Preferred Palette

    int bestID = 0;
    int bestFreq = 0;
    for (auto pair : palettesUsed.asKeyValueRange())
    {
        if (pair.second > bestFreq)
        {
            bestFreq = pair.second;
            bestID = pair.first;
        }
    }
    preferredPalette = bestID;
}

QJsonObject Tile::exportAsJson()
{
    QJsonArray jPixels;
    for (int i=0;i!=64;++i)
        jPixels.append(pixels[i]);

    QJsonObject jPalettes;
    for (auto pair : palettesUsed.asKeyValueRange())
        jPalettes[QString::number(pair.first)] = pair.second;

    QJsonObject jTile;
    jTile["ID"] = id;
    jTile["Pixels"] = jPixels;
    jTile["Palettes"] = jPalettes;
    jTile["PalettesSeen"] = palettesUsed.size();
    jTile["LastSeenOnFrame"] = 0;
    jTile["PaletteSize"] = paletteSize;
    jTile["SeenOnFrames"] = seenOnFrames;
    jTile["UsedInBackground"] = usedInBackground;
    jTile["UsedInSprite"] = usedInSprite;

    return jTile;
}

QByteArray & Tile::uniqueKey()
{
    if (_uniqueKey.isEmpty())
    {
        _uniqueKey.resize(64);

        char * dst = _uniqueKey.data();
        int  * src = pixels;

        for (int i=0;i!=64;++i)
            dst[i] = src[i] & 0xFF;
    }

    return _uniqueKey;
}

//void Tile::import(Tile * other,
//                  QHash<int, int> & oldPaletteID2NewPaletteID,
//                  QHash<int, int> & oldReferenceID2NewReferenceID)
//{
//    for (auto pair : other->palettesUsed.asKeyValueRange())
//    {
//        int newPaletteId = oldPaletteID2NewPaletteID[pair.first];
//        if (palettesUsed.contains(newPaletteId))
//            palettesUsed[newPaletteId] += pair.second;
//        else
//            palettesUsed[newPaletteId] = pair.second;
//    }

//    seenOnFrames += other->seenOnFrames;

//    for (int i=0;i!=3;++i)
//        if (references[i].isEmpty() && !other->references[i].isEmpty())
//            references[i].import(other->references[i], oldPaletteID2NewPaletteID, oldReferenceID2NewReferenceID);
//}

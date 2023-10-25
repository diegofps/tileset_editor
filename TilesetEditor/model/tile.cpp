#include "tile.h"
#include "model/jsonhelpers.h"

Tile::Tile(QJsonObject & data)
{
    getIntOrFail(id, data, "Tile", "ID");
    getIntOrDefault(sceneId, data, "Tile", "SceneID", 0);
    getIntArrayOrFail(pixels, data, "Tile", "Pixels", 64);
    getIntIntQHashOrFail(palettesUsed, data, "Tile", "Palettes");
    getIntOrFail(paletteSize, data, "Tile", "PaletteSize");
    getBoolOrFail(usedInSprite, data, "Tile", "UsedInSprite");
    getBoolOrFail(usedInBackground, data, "Tile", "UsedInBackground");
    getIntOrDefault(seenOnFrames, data, "Tile", "SeenOnFrames", -1);

    getIntOrFail(ref1ID, data, "Tile", "Ref1ID");
    getIntOrFail(ref10ID, data, "Tile", "Ref10ID");
    getIntOrFail(ref100ID, data, "Tile", "Ref100ID");
    getIntOrFail(ref1000ID, data, "Tile", "Ref1000ID");

    getIntOrFail(refNNID, data, "Tile", "RefNNID");
    getIntOrFail(refNFID, data, "Tile", "RefNFID");
    getIntOrFail(refFNID, data, "Tile", "RefFNID");
    getIntOrFail(refFFID, data, "Tile", "RefFFID");

    // Unique key

    _uniqueKey.clear();

    // Preferred Palette

    int bestID = 0;
    int bestFreq = 0;

//    if (id == 169)
//        qDebug() << "Selecting preferred palette for tile " << id;

    for (auto pair : palettesUsed.asKeyValueRange())
    {
//        if (id == 169)
//            qDebug() << "    " << pair.first << ":" << pair.second;

        if (pair.second > bestFreq)
        {
            bestFreq = pair.second;
            bestID = pair.first;
        }
    }

    preferredPalette = bestID;

//    if (id == 169)
//        qDebug() << "Selected palette with id " << preferredPalette;

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
    jTile["SceneID"] = sceneId;
    jTile["Pixels"] = jPixels;
    jTile["Palettes"] = jPalettes;
    jTile["PalettesSeen"] = palettesUsed.size();
    jTile["LastSeenOnFrame"] = 0;
    jTile["PaletteSize"] = paletteSize;
    jTile["SeenOnFrames"] = seenOnFrames;
    jTile["UsedInBackground"] = usedInBackground;
    jTile["UsedInSprite"] = usedInSprite;
    jTile["Ref1ID"] = ref1ID;
    jTile["Ref10ID"] = ref10ID;
    jTile["Ref100ID"] = ref100ID;
    jTile["Ref1000ID"] = ref1000ID;
    jTile["RefNNID"] = refNNID;
    jTile["RefNFID"] = refNFID;
    jTile["RefFNID"] = refFNID;
    jTile["RefFFID"] = refFFID;
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

bool Tile::usedWithHFlip()
{
    return refFNID != 0 || refFFID != 0;
}

bool Tile::usedWithVFlip()
{
    return refNFID != 0 || refFFID != 0;
}



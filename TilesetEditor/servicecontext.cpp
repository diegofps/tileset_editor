#include "servicecontext.h"
#include "app.h"
#include "model/reference.h"
#include "model/screenshot.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLIC INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ServiceContext::ServiceContext()
{

}

void ServiceContext::close()
{
    App::getState()->setContextFolder("");
}

void ServiceContext::load(const QString & folderpath)
{
    App::getState()->setContextFolder(folderpath);
    QDir baseDir(folderpath);

    if (!baseDir.exists("state"))
        baseDir.mkdir("state");

    baseDir.cd("state");

    auto context    = App::getState()->context();
    auto tiles      = App::getState()->contextTiles();
    auto tilesets   = App::getState()->contextTilesets();
    auto palettes   = App::getState()->contextPalettes();
    auto references = App::getState()->contextReferences();

    if (!loadContext(baseDir, context))
        loadEmptyContext(context);

    if (!loadTiles(baseDir, tiles))
        loadEmptyTiles(tiles);

    if (!loadTilesets(baseDir, tilesets))
        loadEmptyTilesets(tilesets);

    if (!loadPalettes(baseDir, palettes))
        loadEmptyPalettes(palettes);

    if (!loadReferences(baseDir, references))
        loadEmptyReferences(references);
}

void ServiceContext::save()
{
    QDir contextDir(App::getState()->contextFolder());

    if (saveContext(contextDir, App::getState()->context()) &&
        saveTiles(contextDir, App::getState()->contextTiles()) &&
        savePalettes(contextDir, App::getState()->contextPalettes()) &&
        saveTilesets(contextDir, App::getState()->contextTilesets()) &&
        saveScreenshots(contextDir, App::getState()->contextScreenshots())
    )
        qInfo("Context saved successfully");
    else
        qWarning("Context failed to save");
}

void ServiceContext::importDump()
{
    QDir dumpDir(App::getState()->contextFolder());
    QDir screenshotsDir(App::getState()->contextFolder());

    if (!dumpDir.cd("dump"))
    {
        qWarning() << "Can't access the dump folder";
        return;
    }

    if (!screenshotsDir.cd("screenshotDump"))
    {
        qWarning() << "Can't access the screenshot folder";
        return;
    }

    QList<Screenshot*> dumpedScreenshots;
    QList<Reference*> dumpedReferences;
    QList<Palette*> dumpedPalettes;
    QList<Tile*> dumpedTiles;

    loadPalettes(dumpDir, &dumpedPalettes);
    loadTiles(dumpDir, &dumpedTiles);
    loadReferences(dumpDir, &dumpedReferences);
    loadScreenshots(screenshotsDir, &dumpedScreenshots);

    QHash<QByteArray,Palette*> statePaletteByKey;
    QHash<QByteArray,Tile*> stateTileByKey;
    QHash<int,int> tileStateId;
    QHash<int,int> paletteStateId;
    QHash<int,int> stateTileNumReferencesById;
    QHash<int,int> screenshotsStateId;

    for (auto item : *App::getState()->contextPalettes())
        statePaletteByKey[item->uniqueKey()] = item;

    for (auto item : *App::getState()->contextTiles())
        stateTileByKey[item->uniqueKey()] = item;

    for (auto ref : *App::getState()->contextReferences())
        ++stateTileNumReferencesById[ref->tileId];

    for (auto dPalette : dumpedPalettes)
    {
        if (statePaletteByKey.contains(dPalette->uniqueKey()))
        {
            paletteStateId[dPalette->id] = statePaletteByKey[dPalette->uniqueKey()]->id;
            delete dPalette;
            continue;
        }

        int const newID = ++App::getState()->context()->lastPaletteID;
        App::getState()->contextPalettes()->append(dPalette);
        paletteStateId[dPalette->id] = newID;
        dPalette->id = newID;
    }

    for (auto dTile : dumpedTiles)
    {
        if (stateTileByKey.contains(dTile->uniqueKey()))
        {
            tileStateId[dTile->id] = stateTileByKey[dTile->uniqueKey()]->id;
            delete dTile;
            continue;
        }

        int const newID = ++App::getState()->context()->lastTileID;
        App::getState()->contextTiles()->append(dTile);
        tileStateId[dTile->id] = newID;
        dTile->id = newID;

        for (int i=0;i!=dTile->palettesUsed.size();++i)
            dTile->palettesUsed[i] = paletteStateId[dTile->palettesUsed[i]];
    }

    for (auto dReference : dumpedReferences)
    {
        if (stateTileNumReferencesById[tileStateId[dReference->tileId]] >= 3)
        {
            delete dReference;
            continue;
        }

        ++stateTileNumReferencesById[tileStateId[dReference->tileId]];

        if (!screenshotsStateId.contains(dReference->screenshotId))
            screenshotsStateId[dReference->screenshotId] = ++App::getState()->context()->lastScreenshotID;

        dReference->screenshotId = screenshotsStateId[dReference->screenshotId];
        dReference->tileId = tileStateId[dReference->tileId];
        App::getState()->contextReferences()->append(dReference);
    }

    for (auto dScreenshot : dumpedScreenshots)
    {
        if (screenshotsStateId.contains(dScreenshot->id))
        {
            dScreenshot->id = screenshotsStateId[dScreenshot->id];
            dScreenshot->filename = QString("%1.png").arg(dScreenshot->id);
            App::getState()->contextScreenshots()->append(dScreenshot);
        }
        else
        {
            delete dScreenshot;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PRIVATE INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// METHODS TO LOAD THE DATA FROM JSON FILES

bool ServiceContext::loadContext(QDir contextDir, Context * context)
{
    QString filename = "context.json";

    if (!contextDir.exists(filename))
    {
        qInfo() << filename << " does not exist";
        return false;
    }

    QFile file = contextDir.filePath(filename);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qWarning() << "Failed to open json file: " << file.fileName();
        return false;
    }

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &errorPtr);
    file.close();

    if (doc.isNull())
    {
        qWarning() << "Failed to parse context: " << file.fileName();
        return false;
    }

    if (!doc.isObject())
    {
        qWarning() << "Json document does not start with an object: " << file.fileName();
        return false;
    }

    context->initFromJson(doc.object());
    return true;
}

template <typename ITEM>
bool loadItems(QDir baseDir, QString name, QList<ITEM*> * items)
{
    QString filename = name + ".json";

    if (!baseDir.exists(filename))
    {
        qInfo() << filename << " does not exist";
        return false;
    }

    QFile file = baseDir.filePath(filename);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qWarning() << "Failed to open json file: " << file.fileName();
        return false;
    }

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &errorPtr);
    file.close();

    if (doc.isNull())
    {
        qWarning() << "Failed to parse " << name << ": " << file.fileName();
        return false;
    }

    if (!doc.isArray())
    {
        qWarning() << "Json document does not start with an array: " << file.fileName();
        return false;
    }

    if (!items->isEmpty())
    {
        for (auto item : *items)
            delete item;

        items->clear();
    }

    QJsonArray array = doc.array();

    for (qsizetype i=0;i!=array.size();++i)
    {
        auto jItem = array.at(i).toObject();
        items->append(new ITEM(jItem));
    }

    qDebug() << "Loaded " << items->size() << " " << name;

    return true;
}

bool ServiceContext::loadTiles(QDir baseDir, QList<Tile*> * tiles)
{
    return loadItems(baseDir, "tiles", tiles);
}

bool ServiceContext::loadPalettes(QDir baseDir, QList<Palette*> * palettes)
{
    return loadItems(baseDir, "palettes", palettes);
}

bool ServiceContext::loadTilesets(QDir baseDir, QList<Tileset*> * tilesets)
{
    return loadItems(baseDir, "tilesets", tilesets);
}

bool ServiceContext::loadScreenshots(QDir baseDir, QList<Screenshot*> * screenshots)
{
    if (!screenshots->isEmpty())
    {
        for (Screenshot * s : *screenshots)
            delete s;
        screenshots->clear();
    }

    for (QString & entry : baseDir.entryList())
    {
        QFile file(baseDir.filePath(entry));

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Failed to open screenshot: " << file.fileName();
            continue;
        }

        auto screenshot = new Screenshot();
        screenshot->data = file.readAll();;
        screenshot->filename = entry;
        screenshot->id = QFileInfo(entry).baseName().toInt();
        screenshots->append(screenshot);
    }

    return true;
}

// METHODS TO INITIALIZE EMPTY STRUCTURES

void ServiceContext::loadEmptyContext(Context * context)
{
    context->clear();
    App::getState()->setContext(context);
}

void ServiceContext::loadEmptyTiles(QList<Tile*> * data)
{
    for (auto tile : *data)
        delete tile;

    data->clear();
    App::getState()->setContextTiles(data);
}

void ServiceContext::loadEmptyTilesets(QList<Tileset*> * data)
{
    for (auto tile : *data)
        delete tile;

    data->clear();

    App::getState()->setContextTilesets(data);
}

void ServiceContext::loadEmptyPalettes(QList<Palette*> * data)
{
    for (auto tile : *data)
        delete tile;

    data->clear();

    App::getState()->setContextPalettes(data);
}

// METHODS TO SAVE THE DATA BACK AS JSON

bool ServiceContext::saveContext(QDir contextFolder, Context * context)
{
    QString const filename = "context.json";
    QFile file = contextFolder.filePath(filename);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qWarning() << "Save error, could not open the output json file for context: " << file.fileName();
        return false;
    }

    QJsonDocument doc(context->exportAsJson());
    file.write(doc.toJson());
    file.close();

    return true;
}

template <typename ITEM>
bool saveItems(QDir contextDir, QString name, QList<ITEM*> * items)
{
    QString const filename = name + ".json";
    QFile file = contextDir.filePath(filename);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qWarning() << "Save error, could not open the output json file for " << name << ": " << file.fileName();
        return false;
    }

    QJsonArray array;
    for (auto item : *items)
        array.append(item->exportAsJson());

    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();

    return true;
}

bool ServiceContext::saveTiles(QDir contextDir, QList<Tile*> * tiles)
{
    return saveItems(contextDir, "tiles", tiles);
}

bool ServiceContext::savePalettes(QDir contextDir, QList<Palette*> * palettes)
{
    return saveItems(contextDir, "palettes", palettes);
}

bool ServiceContext::saveTilesets(QDir contextDir, QList<Tileset*> * tilesets)
{
    return saveItems(contextDir, "tilesets", tilesets);
}

// METHODS TO IMPORT AND MERGE DUMPED DATA

//template <typename ITEM>
//void importItems(QDir contextDir,
//                 QString name,
//                 QList<ITEM*> * items,
//                 bool(*itemsLoader)(QDir, QList<ITEM*>*),
//                 int & lastId)
//{
//    QList<ITEM*> dumpedItems;

//    if (!itemsLoader(contextDir, &dumpedItems))
//    {
//        qInfo() << "The are no " << name << " dumped in this context.";
//        return;
//    }

//    QHash<QByteArray, ITEM*> idToItem;

//    for (auto item : * items)
//        idToItem[item->uniqueKey()] = item;

//    for (auto * dumpedItem : dumpedItems)
//    {
//        if (idToItem.contains(dumpedItem->uniqueKey()))
//        {
//            idToItem[dumpedItem->uniqueKey()]->import(dumpedItem);
//            delete dumpedItem;
//        }
//        else
//        {
//            dumpedItem->setId(++lastId);
//            items->append(dumpedItem);
//            idToItem[dumpedItem->uniqueKey()] = dumpedItem;
//        }
//    }
//}

//void ServiceContext::importDumpedTiles(QDir contextDir,
//                                       QList<Tile*> * items,
//                                       QHash<int, int> & oldPaletteID2NewPaletteID,
//                                       QHash<int, int> & oldReferenceID2NewReferenceID)
//{
//    QList<Tile*> dumpedItems;

//    if (!loadTiles(contextDir, &dumpedItems))
//    {
//        qInfo() << "There are no tiles dumped in this context.";
//        return;
//    }

//    QHash<QByteArray, Tile*> idToItem;

//    for (auto item : * items)
//        idToItem[item->uniqueKey()] = item;

//    for (auto * dumpedItem : dumpedItems)
//    {
//        if (idToItem.contains(dumpedItem->uniqueKey()))
//        {
//            idToItem[dumpedItem->uniqueKey()]->import(dumpedItem, oldPaletteID2NewPaletteID, oldReferenceID2NewReferenceID);
//            delete dumpedItem;
//        }
//        else
//        {
//            items->append(dumpedItem);
//            idToItem[dumpedItem->uniqueKey()] = dumpedItem;
//        }
//    }
//}

//void ServiceContext::importDumpedPalettes(QDir contextDir, QList<Palette*> * palettes)
//{
//    importItems(contextDir, "palettes", palettes, loadPalettes, App::getState()->context()->lastPaletteID);
//}

//void ServiceContext::importDumpedTilesets(QDir contextDir, QList<Tileset*> * tilesets)
//{
//    importItems(contextDir, "tilesets", tilesets, loadTilesets, App::getState()->context()->lastTilesetID);
//}

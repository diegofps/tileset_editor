#include "servicecontext.h"
#include "app.h"

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
    QDir contextDir(folderpath);

    if (!contextDir.exists("state"))
        contextDir.mkdir("state");

    auto context  = App::getState()->context();
    auto tiles    = App::getState()->contextTiles();
    auto tilesets = App::getState()->contextTilesets();
    auto palettes = App::getState()->contextPalettes();

    if (!loadContext(contextDir, context))
        loadEmptyContext(context);

    if (!loadTiles(contextDir, tiles))
        loadEmptyTiles(tiles);

    if (!loadTilesets(contextDir, tilesets))
        loadEmptyTilesets(tilesets);

    if (!loadPalettes(contextDir, palettes))
        loadEmptyPalettes(palettes);
}

void ServiceContext::save()
{
    QDir contextFolder(App::getState()->contextFolder());

    if (saveContext(contextFolder, App::getState()->context()) &&
        saveTiles(contextFolder, App::getState()->contextTiles()) &&
        savePalettes(contextFolder, App::getState()->contextPalettes()) &&
        saveTilesets(contextFolder, App::getState()->contextTilesets())
    )
        qInfo("Context saved successfully");
    else
        qWarning("Context failed to save");
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
bool loadItems(QDir contextDir, QString name, QList<ITEM*> * items)
{
    QString filename = name + ".json";

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
        items->append(new ITEM(array.at(i).toObject()));

    qDebug() << "Loaded " << items->size() << " " << name;

    return true;
}

bool ServiceContext::loadTiles(QDir contextDir, QList<Tile*> * tiles)
{
    return loadItems(contextDir, "tiles", tiles);
}

bool ServiceContext::loadPalettes(QDir contextDir, QList<Palette*> * palettes)
{
    return loadItems(contextDir, "palettes", palettes);
}

bool ServiceContext::loadTilesets(QDir contextDir, QList<Tileset*> * tilesets)
{
    return loadItems(contextDir, "tilesets", tilesets);
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

template <typename ITEM>
void importItems(QDir contextDir, QString name, QList<ITEM*> * items, bool(*itemsLoader)(QDir, QList<ITEM*>*))
{
    QList<ITEM*> dumpedItems;

    if (!itemsLoader(contextDir, &dumpedItems))
    {
        qInfo() << "The are no " << name << " dumped in this context.";
        return;
    }

    QHash<QString, ITEM*> idToItem;

    for (auto item : * items)
        idToItem[item->uniqueKey()] = item;

    for (auto * dumpedItem : dumpedItems)
    {
        if (idToItem.contains(dumpedItem->uniqueKey()))
        {
            idToItem[dumpedItem->uniqueKey()]->import(dumpedItem);
            delete dumpedItem;
        }
        else
        {
            items->append(dumpedItem);
            idToItem[dumpedItem->uniqueKey()] = dumpedItem;
        }
    }
}

void ServiceContext::importDumpedTiles(QDir contextDir, QList<Tile*> * tiles)
{
    importItems(contextDir, "tiles", tiles, loadTiles);
}

void ServiceContext::importDumpedPalettes(QDir contextDir, QList<Palette*> * palettes)
{
    importItems(contextDir, "palettes", palettes, loadPalettes);
}

void ServiceContext::importDumpedTilesets(QDir contextDir, QList<Tileset*> * tilesets)
{
    importItems(contextDir, "tilesets", tilesets, loadTilesets);
}

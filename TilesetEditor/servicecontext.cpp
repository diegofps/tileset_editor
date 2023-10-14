#include "servicecontext.h"

#include "app.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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
    QDir directorypath(folderpath);

    if (!directorypath.exists("state"))
        directorypath.mkdir("state");

    auto context = App::getState()->context();
    auto tiles = App::getState()->contextTiles();
    auto tilesets = App::getState()->contextTilesets();
    auto palettes = App::getState()->contextPalettes();

    if (!loadContext(directorypath, context))
        loadEmptyContext(context);

    if (!loadTiles(directorypath, tiles))
        loadEmptyTiles(tiles);

    if (!loadTilesets(directorypath, tilesets))
        loadEmptyTilesets(tilesets);

    if (!loadPalettes(directorypath, palettes))
        loadEmptyPalettes(palettes);
}

void ServiceContext::save()
{
    saveTiles(App::getState()->contextTiles());
    savePalettes(App::getState()->contextPalettes());
    saveTilesets(App::getState()->contextTilesets());
}

// LOAD FROM JSON METHODS

template <typename ITEM>
bool loadItems(QDir directorypath, QString name, QList<ITEM*> * items)
{
    QString filename = name + ".json";
    if (!directorypath.exists(filename))
    {
        qInfo() << filename << " does not exist";
        return false;
    }

    QFile file = directorypath.filePath(filename);

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

    for (auto item : *items)
        delete item;

    items->clear();

    QJsonArray array = doc.array();

    for (qsizetype i=0;i!=array.size();++i)
        items->append(new ITEM(array.at(i).toObject()));

    qDebug() << "Loaded " << items->size() << " " << name;

    return true;
}

bool ServiceContext::loadTiles(QDir directorypath, QList<Tile*> * tiles)
{
    return loadItems(directorypath, "tiles", tiles);
}

bool ServiceContext::loadPalettes(QDir directorypath, QList<Palette*> * palettes)
{
    return loadItems(directorypath, "palettes", palettes);
}

bool ServiceContext::loadTilesets(QDir directorypath, QList<Tileset*> * tilesets)
{
    return loadItems(directorypath, "tilesets", tilesets);
}

// LOAD EMPTY DATA

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

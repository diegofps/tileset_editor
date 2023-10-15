#ifndef SERVICECONTEXT_H
#define SERVICECONTEXT_H

#include <QString>
#include <QDir>

#include "model/context.h"
#include "model/reference.h"
#include "model/screenshot.h"
#include "model/tile.h"
#include "model/palette.h"
#include "model/tileset.h"

class ServiceContext
{
public:

    ServiceContext();

    static void close();
    static void load(QString const & folderpath);
    static void save();
    static void importDump();

private:

    static bool loadContext(QDir contextDir, Context * context);
    static bool loadTiles(QDir contextDir, QList<Tile*> * tiles);
    static bool loadPalettes(QDir contextDir, QList<Palette*> * palettes);
    static bool loadTilesets(QDir contextDir, QList<Tileset*> * tilesets);
    static bool loadScreenshots(QDir baseDir, QList<Screenshot*> * screenshots);
    static bool loadReferences(QDir baseDir, QList<Reference*> * references);

    static void loadEmptyContext(Context * context);
    static void loadEmptyTiles(QList<Tile*> * tiles);
    static void loadEmptyPalettes(QList<Palette*> * palettes);
    static void loadEmptyTilesets(QList<Tileset*> * tilesets);
    static void loadEmptyReferences(QList<Reference*> * references);

    static bool saveContext(QDir contextDir, Context * context);
    static bool saveTiles(QDir contextDir, QList<Tile*> * tiles);
    static bool savePalettes(QDir contextDir, QList<Palette*> * palettes);
    static bool saveTilesets(QDir contextDir, QList<Tileset*> * tilesets);
    static bool saveReferences(QDir contextDir, QList<Reference*> * references);
    static bool saveScreenshots(QDir contextDir, QList<Screenshot*> * screenshots);

    static void importDumpedTiles(QDir contextDir, QList<Tile*> * tiles);
    static void importDumpedPalettes(QDir contextDir, QList<Palette*> * palettes);
    static void importDumpedTilesets(QDir contextDir, QList<Tileset*> * tilesets);
    static void importDumpedReferences(QDir contextDir, QList<Reference*> * references);
//    static void importDumpedRefScreenshots(QDir contextDir); // This will be performed during importDumpedTiles, as we won't import all of them

};

#endif // SERVICECONTEXT_H

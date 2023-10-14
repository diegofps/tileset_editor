#ifndef SERVICECONTEXT_H
#define SERVICECONTEXT_H

#include <QString>
#include <QDir>

#include "model/context.h"
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

    static bool loadContext(QDir directorypath, Context * context);
    static bool loadTiles(QDir directorypath, QList<Tile*> * tiles);
    static bool loadPalettes(QDir directorypath, QList<Palette*> * palettes);
    static bool loadTilesets(QDir directorypath, QList<Tileset*> * tilesets);

    static void loadEmptyContext(Context * context);
    static void loadEmptyTiles(QList<Tile*> * tiles);
    static void loadEmptyPalettes(QList<Palette*> * palettes);
    static void loadEmptyTilesets(QList<Tileset*> * tilesets);

    static void saveContext(Context * context);
    static void saveTiles(QList<Tile*> * tiles);
    static void savePalettes(QList<Palette*> * palettes);
    static void saveTilesets(QList<Tileset*> * tilesets);

    static void importDumpedTiles();
    static void importDumpedPalettes();
    static void importDumpedTilesets();
    static void importDumpedRefScreenshots();

};

#endif // SERVICECONTEXT_H

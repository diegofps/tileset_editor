#ifndef SERVICECONTEXT_H
#define SERVICECONTEXT_H

#include <QString>
#include <QDir>

#include "model/scene.h"
#include "model/project.h"
#include "model/reference.h"
#include "model/screenshot.h"
#include "model/tile.h"
#include "model/palette.h"
#include "model/tileset.h"

class ContextReport
{
private:
    bool _success;
    QString _message;
public:
    void success(QString message);
    void fail(QString message);
    bool success() const;
    const QString &message() const;
};

class ServiceContext
{
public:

    ServiceContext();

    static void create(QString const & folderpath, ContextReport * report = nullptr);
    static void close(ContextReport * report = nullptr);
    static void load(QString const & folderpath, ContextReport * report = nullptr);
    static void save(ContextReport * report = nullptr);
    static void importDump(QString const & folderpath, ContextReport * report = nullptr);

private:

    static bool loadContext(QDir contextDir, Project * context);
    static bool loadTiles(QDir contextDir, QList<Tile*> * tiles);
    static bool loadPalettes(QDir contextDir, QList<Palette*> * palettes);
    static bool loadTilesets(QDir contextDir, QList<Tileset*> * tilesets);
    static bool loadScreenshots(QDir baseDir, QList<Screenshot*> * screenshots);
    static bool loadReferences(QDir baseDir, QList<Reference*> * references);
    static bool loadScenes(QDir baseDir, QList<Scene*> * scenes);

    static bool saveContext(QDir contextDir, Project * context);
    static bool saveTiles(QDir contextDir, QList<Tile*> * tiles);
    static bool savePalettes(QDir contextDir, QList<Palette*> * palettes);
    static bool saveTilesets(QDir contextDir, QList<Tileset*> * tilesets);
    static bool saveReferences(QDir contextDir, QList<Reference*> * references);
    static bool saveScreenshots(QDir contextDir, QList<Screenshot*> * screenshots);
    static bool saveScenes(QDir contextDir, QList<Scene*> * scenes);

};

#endif // SERVICECONTEXT_H

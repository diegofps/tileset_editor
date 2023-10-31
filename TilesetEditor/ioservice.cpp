#include "ioservice.h"
#include "app.h"
#include "errors.h"
#include "model/reference.h"
#include "model/screenshot.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include <QImage>


#define FAIL(msg) {\
    qWarning() << (msg);\
    if (report != nullptr)\
        report->fail(msg);\
}

#define ABORT(msg) {\
    qWarning() << (msg);\
    if (report != nullptr)\
        report->fail(msg);\
    return;\
}

#define SUCCESS(msg) {\
    qInfo() << (msg);\
    if (report != nullptr)\
        report->success(msg);\
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLIC INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IOService::IOService()
{

}

void IOService::create(const QString & folderpath, IOReport * report)
{
    qInfo() << "Creating project";

    Project* project = new Project();
    QList<Tile*>* tiles = new QList<Tile*>();
    QList<Tileset*>* tilesets = new QList<Tileset*>();
    QList<Palette*>* palettes = new QList<Palette*>();
    QList<Reference*>* references = new QList<Reference*>();
    QList<Screenshot*>* screenshots = new QList<Screenshot*>();
    QList<Scene*>* scenes = new QList<Scene*>();

    project->path = folderpath;

    auto ts = new Tileset();
    ts->id = ++project->lastTilesetID;
    ts->name = "Noname";
    ts->sceneId = 0;
    ts->gridW = 10;
    ts->gridH = 10;
    ts->bgColor = QColor::fromRgb(0,0,0);
    tilesets->append(ts);

    App::getState()->setAllTiles(tiles);
    App::getState()->setAllTilesets(tilesets);
    App::getState()->setAllPalettes(palettes);
    App::getState()->setAllReferences(references);
    App::getState()->setAllScreenshots(screenshots);
    App::getState()->setAllScenes(scenes);
    App::getState()->setProjectHasChanges(false);
    App::getState()->setProject(project);
    App::getOriginalTileCache()->clear();

    save(report);
}

void IOService::close(IOReport * report)
{
    qInfo() << "Closing project";

    auto project = App::getState()->project();

    if (project == nullptr)
        ABORT("Project is already closed");

    QString path = project->path;
    QList<qsizetype> emptyTiles;

    App::getState()->setProject(nullptr);
    App::getState()->setSelectedTileset(nullptr);
    App::getState()->setSelectedTiles(emptyTiles);
    App::getState()->setAllPalettes(nullptr);
    App::getState()->setAllTiles(nullptr);
    App::getState()->setAllTilesets(nullptr);
    App::getState()->setAllReferences(nullptr);
    App::getState()->setAllScreenshots(nullptr);
    App::getState()->setAllScenes(nullptr);
    App::getOriginalTileCache()->clear();

    SUCCESS(QString("Project %1 closed successfully").arg(path));
}

void IOService::load(const QString & folderpath, IOReport * report)
{
    qInfo() << "Loading project";

    Project* project = new Project();
    QList<Tile*>* tiles = new QList<Tile*>();
    QList<Tileset*>* tilesets = new QList<Tileset*>();
    QList<Palette*>* palettes = new QList<Palette*>();
    QList<Reference*>* references = new QList<Reference*>();
    QList<Screenshot*>* screenshots = new QList<Screenshot*>();
    QList<Scene*>* scenes = new QList<Scene*>();

    QDir baseDir(folderpath);

    if (loadProject(baseDir, project) &&
        loadTiles(baseDir, tiles) &&
        loadTilesets(baseDir, tilesets) &&
        loadPalettes(baseDir, palettes) &&
        loadReferences(baseDir, references) &&
        loadScenes(baseDir, scenes)
    )
    {
        close();

        App::getState()->setAllTiles(tiles);
        App::getState()->setAllTilesets(tilesets);
        App::getState()->setAllPalettes(palettes);
        App::getState()->setAllReferences(references);
        App::getState()->setAllScreenshots(screenshots);
        App::getState()->setAllScenes(scenes);
        App::getState()->setProjectHasChanges(false);
        App::getState()->setProject(project);

        App::getState()->updateFilteredPalettes();
        App::getState()->updateFilteredTiles();
        App::getState()->updateFilteredTilesets();

        SUCCESS(QString("Project loaded successfully: %1").arg(folderpath));
    }
    else
    {
        for (auto item : *tiles)
            delete item;

        for (auto item : *tilesets)
            delete item;

        for (auto item : *palettes)
            delete item;

        for (auto item : *references)
            delete item;

        for (auto item : *screenshots)
            delete item;

        for (auto item : *scenes)
            delete item;

        delete project;
        delete tiles;
        delete tilesets;
        delete palettes;
        delete references;
        delete screenshots;
        delete scenes;

        FAIL(QString("Failed to load project: %1").arg(folderpath));
    }
}

void IOService::save(IOReport * report)
{
    qInfo() << "Saving project";
    // TODO: Prevent partial save on errors

    QDir projectDir(App::getState()->project()->path);

    if (!projectDir.exists())
        projectDir.mkpath(".");

    if (saveContext(projectDir, App::getState()->project()) &&
        saveTiles(projectDir, App::getState()->allTiles()) &&
        savePalettes(projectDir, App::getState()->allPalettes()) &&
        saveReferences(projectDir, App::getState()->allReferences()) &&
        saveTilesets(projectDir, App::getState()->allTilesets()) &&
        saveScreenshots(projectDir, App::getState()->allScreenshots()) &&
        saveScenes(projectDir, App::getState()->allScenes()) )
    {
        App::getState()->setProjectHasChanges(false);

        qInfo("Context saved successfully.");
        SUCCESS(QString("Project saved at %1").arg(projectDir.absolutePath()));
    }
    else
    {
        FAIL("Failed to save project.");
    }
}

void IOService::importDump(QString const & folderpath, IOReport * report)
{
    qInfo() << "Importing dump";
    // TODO: Prevent partial import on errors

    QDir dumpDir(folderpath);
    QDir screenshotsDir(folderpath);

    if (!screenshotsDir.cd("screenshots"))
        ABORT("This is not a valid dump directory, missing screenshots folder.");

    int tilesImported = 0;
    int palettesImported = 0;
    int referencesImported = 0;
    int screenshotsImported = 0;

    QList<Screenshot*> dumpedScreenshots;
    QList<Reference*> dumpedReferences;
    QList<Palette*> dumpedPalettes;
    QList<Tile*> dumpedTiles;

    if (!loadPalettes(dumpDir, &dumpedPalettes) ||
        !loadTiles(dumpDir, &dumpedTiles) ||
        !loadReferences(dumpDir, &dumpedReferences) ||
        !loadScreenshots(screenshotsDir, &dumpedScreenshots) )
        ABORT("This is not a valid dump directory, can't load dumped elements.");

    qInfo() << "Dumped folder has" << dumpedTiles.size() << "tiles,"
            << dumpedPalettes.size() << "palettes,"
            << dumpedReferences.size() << "references, and"
            << dumpedScreenshots.size() << "screenshots.";

    std::sort(dumpedTiles.begin(), dumpedTiles.end(), [](Tile * const a, Tile * const b) {
        return a->id < b->id;
    });

    std::sort(dumpedPalettes.begin(), dumpedPalettes.end(), [](Palette * const a, Palette * const b) {
        return a->id < b->id;
    });

    std::sort(dumpedReferences.begin(), dumpedReferences.end(), [](Reference * const a, Reference * const b) {
        return a->id < b->id;
    });

    std::sort(dumpedScreenshots.begin(), dumpedScreenshots.end(), [](Screenshot * const a, Screenshot * const b) {
        return a->id < b->id;
    });

    QHash<QByteArray,Palette*> statePaletteByKey;
    QHash<QByteArray,Tile*> stateTileByKey;
    QHash<int,int> tileStateId;
    QHash<int,int> paletteStateId;
    QHash<int,int> referencesStateId;
    QHash<int,int> screenshotsStateId;

    for (auto item : *App::getState()->allPalettes())
        statePaletteByKey[item->uniqueKey()] = item;

    for (auto item : *App::getState()->allTiles())
        stateTileByKey[item->uniqueKey()] = item;

    for (auto dPalette : dumpedPalettes)
    {
        if (statePaletteByKey.contains(dPalette->uniqueKey()))
        {
            paletteStateId[dPalette->id] = statePaletteByKey[dPalette->uniqueKey()]->id;
            delete dPalette;
            continue;
        }

        int const newID = ++App::getState()->project()->lastPaletteID;
        paletteStateId[dPalette->id] = newID;
        dPalette->id = newID;

        App::getState()->addPalette(dPalette);
        ++palettesImported;
    }

#define IMPORT_REFERENCE_1(refName) \
    if (dTile->refName != 0 && tile->refName == 0) \
    { \
        int const newID = ++App::getState()->project()->lastReferenceID; \
        referencesStateId.emplace(dTile->refName, newID); \
        tile->refName = newID; \
    }

#define IMPORT_REFERENCE_2(refName) \
    if (dTile->refName != 0) \
    { \
        int const newID = ++App::getState()->project()->lastReferenceID; \
        referencesStateId.emplace(dTile->refName, newID); \
        dTile->refName = newID; \
    }

    for (auto dTile : dumpedTiles)
    {
        if (stateTileByKey.contains(dTile->uniqueKey()))
        {
            auto tile = stateTileByKey[dTile->uniqueKey()];
            tileStateId[dTile->id] = tile->id;

            if (dTile->usedInSprite)
                tile->usedInSprite= true;

            if (dTile->usedInBackground)
                tile->usedInBackground = true;

            tile->seenOnFrames += dTile->seenOnFrames;

//            for (auto pair : dTile->palettesUsed.asKeyValueRange())
//                tile->palettesUsed[pair.first] += pair.second;

            IMPORT_REFERENCE_1(ref1ID);
            IMPORT_REFERENCE_1(ref25ID);
            IMPORT_REFERENCE_1(ref50ID);
            IMPORT_REFERENCE_1(ref75ID);
            IMPORT_REFERENCE_1(ref100ID);
            IMPORT_REFERENCE_1(refNNID);
            IMPORT_REFERENCE_1(refNFID);
            IMPORT_REFERENCE_1(refFNID);
            IMPORT_REFERENCE_1(refFFID);

            delete dTile;
            continue;
        }
        else
        {
            int const newID = ++App::getState()->project()->lastTileID;
            tileStateId[dTile->id] = newID;
            dTile->id = newID;

            IMPORT_REFERENCE_2(ref1ID);
            IMPORT_REFERENCE_2(ref25ID);
            IMPORT_REFERENCE_2(ref50ID);
            IMPORT_REFERENCE_2(ref75ID);
            IMPORT_REFERENCE_2(ref100ID);
            IMPORT_REFERENCE_2(refNNID);
            IMPORT_REFERENCE_2(refNFID);
            IMPORT_REFERENCE_2(refFNID);
            IMPORT_REFERENCE_2(refFFID);

            QHash<int, int> newPalettesUsed;
            for (auto pair : dTile->palettesUsed.asKeyValueRange())
                newPalettesUsed[paletteStateId[pair.first]] = pair.second;
            dTile->palettesUsed = std::move(newPalettesUsed);

            App::getState()->addTile(dTile);
            ++tilesImported;
        }
    }

    for (auto dReference : dumpedReferences)
    {
        if (!referencesStateId.contains(dReference->id))
        {
            delete dReference;
            continue;
        }

        if (!screenshotsStateId.contains(dReference->screenshotId))
            screenshotsStateId[dReference->screenshotId] = ++App::getState()->project()->lastScreenshotID;

        dReference->id = referencesStateId[dReference->id];
        dReference->screenshotId = screenshotsStateId[dReference->screenshotId];
        dReference->colorPaletteID = paletteStateId[dReference->colorPaletteID];

        App::getState()->addReference(dReference);
        ++referencesImported;
    }

    for (auto dScreenshot : dumpedScreenshots)
    {
        if (!screenshotsStateId.contains(dScreenshot->id))
        {
            delete dScreenshot;
            continue;
        }

        QFile file(screenshotsDir.filePath(dScreenshot->filename));

        if (!file.open(QIODevice::ReadOnly))
            qWarning() << "Failed to open required screenshot file: " << file.fileName();

        dScreenshot->id = screenshotsStateId[dScreenshot->id];
        dScreenshot->filename = QString::number(dScreenshot->id).rightJustified(6, '0') + ".png";
        dScreenshot->data = file.readAll();

        App::getState()->addScreenshot(dScreenshot);
        ++screenshotsImported;
    }

    App::getState()->updateFilteredTiles();
    App::getState()->setProject(App::getState()->project());
    App::getState()->setProjectHasChanges(true);

    SUCCESS(QString("Dump imported, new items: tiles=%1, palettes=%2, references=%3, screenshots=%4.")
            .arg(tilesImported)
            .arg(palettesImported)
            .arg(referencesImported)
            .arg(screenshotsImported))
}

void IOService::buildTilesets(IOReport * report)
{
    QDir tilesetsDir(App::getState()->project()->path);
    char const * foldername = "tilesets.low";

    if (tilesetsDir.exists(foldername))
    {
        QDir tmp = tilesetsDir;
        if (!tmp.cd(foldername) || !tmp.removeRecursively())
            ABORT("Could not remove previous tilesets output folder");
    }

    if (!tilesetsDir.mkdir(foldername))
        ABORT("Could not create new tilesets output folder");

    if (!tilesetsDir.cd(foldername))
        ABORT("Could not access tilesets output folder");

    QBrush brushBackground;
    brushBackground.setStyle(Qt::SolidPattern);

    for (auto tileset : *App::getState()->allTilesets())
    {
        auto state = App::getState();
        auto tileCache = App::getOriginalTileCache();

        QString filename = QString("%1_%2.png").arg(tileset->sceneId).arg(tileset->id);
        QString filepath = tilesetsDir.filePath(filename);
        QFile file(filepath);

        if (file.exists() && !file.remove())
            ABORT(QString("Failed to remove previous tileset file: %1").arg(filename));

        QImage img(tileset->gridW*8, tileset->gridH*8, QImage::Format_ARGB32);
        QPainter painter(&img);

        brushBackground.setColor(tileset->bgColor);
        painter.fillRect(img.rect(), brushBackground);

        for (auto pair : tileset->cells.asKeyValueRange())
        {
            auto cell = pair.second;
            auto tile = state->getTileById(cell->tileID);
            auto palette = state->getPaletteById(cell->paletteID);

            QPixmap * pixmap = tileCache->getTilePixmap(tile, palette, cell->hFlip, cell->vFlip);

            if (pixmap == nullptr)
                continue;

            painter.drawPixmap(QRect(cell->x*8, cell->y*8, 8, 8), *pixmap);
        }

        img.save(filepath);
    }

    SUCCESS("Tilesets generated successfully");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PRIVATE INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// METHODS TO LOAD THE DATA FROM JSON FILES

bool IOService::loadProject(QDir projectDir, Project * project)
{
    project->path = projectDir.absolutePath();

    QString filename = "project.json";

    if (!projectDir.exists(filename))
    {
        qInfo() << filename << " does not exist";
        return false;
    }

    QFile file = projectDir.filePath(filename);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qWarning() << "Failed to open project file: " << file.fileName();
        return false;
    }

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &errorPtr);
    file.close();

    if (doc.isNull())
    {
        qWarning() << "Failed to parse project file: " << file.fileName();
        return false;
    }

    if (!doc.isObject())
    {
        qWarning() << "Project file does not start with an object: " << file.fileName();
        return false;
    }

    try {
        if (!project->initFromJson(doc.object()))
        {
            qWarning() << "Project is not valid: " << file.fileName();
            return false;
        }
    } catch (ProjectError & e) {
        qWarning() << e.msg();
        return false;
    }


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

    try
    {
        for (qsizetype i=0;i!=array.size();++i)
        {
            auto jItem = array.at(i).toObject();
            items->append(new ITEM(jItem));
        }
    }
    catch (ProjectError & e)
    {
        qWarning() << "ContextError during loadItems: " << e.msg();
        return false;
    }

    return true;
}

bool IOService::loadTiles(QDir baseDir, QList<Tile*> * tiles)
{
    return loadItems(baseDir, "tiles", tiles);
}

bool IOService::loadPalettes(QDir baseDir, QList<Palette*> * palettes)
{
    return loadItems(baseDir, "palettes", palettes);
}

bool IOService::loadReferences(QDir baseDir, QList<Reference*> * references)
{
    return loadItems(baseDir, "references", references);
}

bool IOService::loadScenes(QDir baseDir, QList<Scene*> * scenes)
{
    return loadItems(baseDir, "scenes", scenes);
}

bool IOService::loadTilesets(QDir baseDir, QList<Tileset*> * tilesets)
{
    return loadItems(baseDir, "tilesets", tilesets);
}

bool IOService::loadScreenshots(QDir baseDir, QList<Screenshot*> * screenshots)
{
    if (!screenshots->isEmpty())
    {
        for (Screenshot * s : *screenshots)
            delete s;
        screenshots->clear();
    }

    for (QString & entry : baseDir.entryList())
    {
        auto screenshot = new Screenshot();
        screenshot->filename = entry;
        screenshot->id = QFileInfo(entry).baseName().toInt();
        screenshots->append(screenshot);
    }

    return true;
}

// METHODS TO SAVE THE DATA BACK AS JSON

bool IOService::saveContext(QDir projectDir, Project * project)
{
    QString const filename = "project.json";
    QFile file = projectDir.filePath(filename);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qWarning() << "Save error, could not open the output json file for project file: " << file.fileName();
        return false;
    }

    QJsonDocument doc(project->exportAsJson());
    file.write(doc.toJson());
    file.close();

    return true;
}

template <typename ITEM>
bool saveItems(QDir projectDir, QString name, QList<ITEM*> * items)
{
    QString const filename = name + ".json";
    QFile file = projectDir.filePath(filename);

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

bool IOService::saveTiles(QDir projectDir, QList<Tile*> * tiles)
{
    return saveItems(projectDir, "tiles", tiles);
}

bool IOService::savePalettes(QDir projectDir, QList<Palette*> * palettes)
{
    return saveItems(projectDir, "palettes", palettes);
}

bool IOService::saveTilesets(QDir projectDir, QList<Tileset*> * tilesets)
{
    return saveItems(projectDir, "tilesets", tilesets);
}

bool IOService::saveReferences(QDir projectDir, QList<Reference*> * references)
{
    return saveItems(projectDir, "references", references);
}

bool IOService::saveScenes(QDir projectDir, QList<Scene*> * scenes)
{
    return saveItems(projectDir, "scenes", scenes);
}

bool IOService::saveScreenshots(QDir projectDir, QList<Screenshot*> * screenshots)
{
    if (!projectDir.exists("screenshots") && !projectDir.mkdir("screenshots"))
    {
        qWarning() << "Save error, could not create screenshots folder.";
        return false;
    }

    if  (!projectDir.cd("screenshots"))
    {
        qWarning() << "Save error, could not access the screenshots folder.";
        return false;
    }

    for (auto screenshot : *screenshots)
    {
        QFile file = projectDir.filePath(screenshot->filename);

        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << "Save error, could not open the output screenshot file for " << screenshot->filename;
            return false;
        }

        file.write(screenshot->data);
    }

    screenshots->clear();

    return true;
}


// CONTEXT_REPORT


IOReport::IOReport() :
    _success(true)
{

}

void IOReport::success(QString const message)
{
//    qInfo() << message;
    _success = true;
    _message = message;
}

void IOReport::fail(QString const message)
{
//    qWarning() << message;
    _success = false;
    _message = message;
}

bool IOReport::success() const
{
    return _success;
}

const QString &IOReport::message() const
{
    return _message;
}
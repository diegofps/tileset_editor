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

void ServiceContext::create(const QString & folderpath, ContextReport * report)
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
    ts->gridW = 10;
    ts->gridH = 10;
    ts->bgColor = QColor::fromRgb(0,0,0);
    tilesets->append(ts);

    App::getState()->setProjectTiles(tiles);
    App::getState()->setProjectTilesets(tilesets);
    App::getState()->setProjectPalettes(palettes);
    App::getState()->setProjectReferences(references);
    App::getState()->setProjectScreenshots(screenshots);
    App::getState()->setProjectScenes(scenes);
    App::getState()->setProjectHasChanges(false);
    App::getState()->setProject(project);
    App::getOriginalTileCache()->clear();

    save(report);
}

void ServiceContext::close(ContextReport * report)
{
    qInfo() << "Closing project";

    auto project = App::getState()->project();

    if (project == nullptr)
    {
        qWarning() << "Project is already closed";
        if (report != nullptr)
            report->fail("Project is already closed");
        return;
    }

    QString path = project->path;

    App::getState()->setProject(nullptr);
    App::getState()->setProjectTiles(nullptr);
    App::getState()->setProjectTilesets(nullptr);
    App::getState()->setProjectPalettes(nullptr);
    App::getState()->setProjectReferences(nullptr);
    App::getState()->setProjectScreenshots(nullptr);
    App::getState()->setProjectScenes(nullptr);
    App::getOriginalTileCache()->clear();

    if (report != nullptr)
        report->success(QString("Project %1 closed successfully").arg(path));
}

void ServiceContext::load(const QString & folderpath, ContextReport * report)
{
    qInfo() << "Loading project";

    Project* context = new Project();
    QList<Tile*>* tiles = new QList<Tile*>();
    QList<Tileset*>* tilesets = new QList<Tileset*>();
    QList<Palette*>* palettes = new QList<Palette*>();
    QList<Reference*>* references = new QList<Reference*>();
    QList<Screenshot*>* screenshots = new QList<Screenshot*>();
    QList<Scene*>* scenes = new QList<Scene*>();

    QDir baseDir(folderpath);

    if (loadContext(baseDir, context) &&
        loadTiles(baseDir, tiles) &&
        loadTilesets(baseDir, tilesets) &&
        loadPalettes(baseDir, palettes) &&
        loadReferences(baseDir, references) &&
        loadScenes(baseDir, scenes)
    )
    {
        App::getState()->setProjectTiles(tiles);
        App::getState()->setProjectTilesets(tilesets);
        App::getState()->setProjectPalettes(palettes);
        App::getState()->setProjectReferences(references);
        App::getState()->setProjectScreenshots(screenshots);
        App::getState()->setProjectScenes(scenes);
        App::getState()->setProjectHasChanges(false);
        App::getState()->setProject(context);
        App::getOriginalTileCache()->clear();

        if (report != nullptr)
            report->success(QString("Project loaded successfully: %1").arg(folderpath));
    }
    else
    {
        if (report != nullptr)
            report->fail(QString("Failed to load project: %1").arg(folderpath));

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

        delete context;
        delete tiles;
        delete tilesets;
        delete palettes;
        delete references;
        delete screenshots;
        delete scenes;
    }
}

void ServiceContext::save(ContextReport * report)
{
    qInfo() << "Saving project";
    // TODO: Prevent partial save on errors

    QDir contextDir(App::getState()->project()->path);

    if (!contextDir.exists())
        contextDir.mkpath(".");

    if (saveContext(contextDir, App::getState()->project()) &&
        saveTiles(contextDir, App::getState()->projectTiles()) &&
        savePalettes(contextDir, App::getState()->projectPalettes()) &&
        saveReferences(contextDir, App::getState()->projectReferences()) &&
        saveTilesets(contextDir, App::getState()->projectTilesets()) &&
        saveScreenshots(contextDir, App::getState()->projectScreenshots()) &&
        saveScenes(contextDir, App::getState()->projectScenes()) )
    {
        App::getState()->setProjectHasChanges(false);

        qInfo("Context saved successfully.");
        if (report != nullptr)
            report->success(QString("Project saved at %1").arg(contextDir.absolutePath()));
    }
    else
    {
        qWarning("Context failed to save");
        if (report != nullptr)
            report->fail("Failed to save context.");
    }
}

void ServiceContext::importDump(QString const & folderpath, ContextReport * report)
{
    qInfo() << "Importing dump";
    // TODO: Prevent partial import on errors

    QDir dumpDir(folderpath);
    QDir screenshotsDir(folderpath);

    if (!screenshotsDir.cd("screenshots"))
    {
        qWarning() << "Can't access the screenshot folder";
        if (report != nullptr)
            report->fail("This is not a valid dump directory, missing screenshots folder.");
        return;
    }

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
    {
        qWarning() << "Can't load dumped files";
        if (report != nullptr)
            report->fail("This is not a valid dump directory, can't load dumped elements.");
        return;
    }

    qInfo() << "Dumped folder has" << dumpedTiles.size() << "tiles,"
            << dumpedPalettes.size() << "palettes,"
            << dumpedReferences.size() << "references, and"
            << dumpedScreenshots.size() << "screenshots.";

    QHash<QByteArray,Palette*> statePaletteByKey;
    QHash<QByteArray,Tile*> stateTileByKey;
    QHash<int,int> tileStateId;
    QHash<int,int> paletteStateId;
    QHash<int,int> stateTileNumReferencesById;
    QHash<int,int> screenshotsStateId;

    for (auto item : *App::getState()->projectPalettes())
        statePaletteByKey[item->uniqueKey()] = item;

    for (auto item : *App::getState()->projectTiles())
        stateTileByKey[item->uniqueKey()] = item;

    for (auto ref : *App::getState()->projectReferences())
        ++stateTileNumReferencesById[ref->tileId];

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

        App::getState()->appendProjectPalette(dPalette);
        ++palettesImported;
    }

    for (auto dTile : dumpedTiles)
    {
        if (stateTileByKey.contains(dTile->uniqueKey()))
        {
            tileStateId[dTile->id] = stateTileByKey[dTile->uniqueKey()]->id;
            delete dTile;
            continue;
        }

        int const newID = ++App::getState()->project()->lastTileID;
        tileStateId[dTile->id] = newID;
        dTile->id = newID;

        QHash<int, int> newPalettesUsed;
        for (auto pair : dTile->palettesUsed.asKeyValueRange())
            newPalettesUsed[paletteStateId[pair.first]] = pair.second;
        dTile->palettesUsed = std::move(newPalettesUsed);

        App::getState()->appendProjectTile(dTile);
        ++tilesImported;
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
            screenshotsStateId[dReference->screenshotId] = ++App::getState()->project()->lastScreenshotID;

        dReference->id = ++App::getState()->project()->lastReferenceID;
        dReference->screenshotId = screenshotsStateId[dReference->screenshotId];
        dReference->tileId = tileStateId[dReference->tileId];

        App::getState()->appendProjectReference(dReference);
        ++referencesImported;
    }

    for (auto dScreenshot : dumpedScreenshots)
    {
        if (screenshotsStateId.contains(dScreenshot->id))
        {
            QFile file(screenshotsDir.filePath(dScreenshot->filename));

            if (!file.open(QIODevice::ReadOnly))
                qWarning() << "Failed to open required screenshot file: " << file.fileName();

            dScreenshot->id = screenshotsStateId[dScreenshot->id];
            dScreenshot->filename = QString::number(dScreenshot->id).rightJustified(6, '0') + ".png";
            dScreenshot->data = file.readAll();

            App::getState()->appendProjectScreenshot(dScreenshot);
            ++screenshotsImported;
        }
        else
        {
            delete dScreenshot;
        }
    }

    auto project = App::getState()->project();

    App::getState()->setProject(project);
    App::getState()->setProjectHasChanges(true);

    if (report != nullptr)
        report->success(QString("Dump imported, new items: tiles=%1, palettes=%2, references=%3, screenshots=%4.")
                        .arg(tilesImported).arg(palettesImported).arg(referencesImported).arg(screenshotsImported));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PRIVATE INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// METHODS TO LOAD THE DATA FROM JSON FILES

bool ServiceContext::loadContext(QDir contextDir, Project * context)
{
    context->path = contextDir.absolutePath();

    QString filename = "context.json";

    if (!contextDir.exists(filename))
    {
        qInfo() << filename << " does not exist";
        return false;
    }

    QFile file = contextDir.filePath(filename);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qWarning() << "Failed to open context file: " << file.fileName();
        return false;
    }

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &errorPtr);
    file.close();

    if (doc.isNull())
    {
        qWarning() << "Failed to parse context file: " << file.fileName();
        return false;
    }

    if (!doc.isObject())
    {
        qWarning() << "Context file does not start with an object: " << file.fileName();
        return false;
    }

    if (!context->initFromJson(doc.object()))
    {
        qWarning() << "Context is not valid: " << file.fileName();
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

    for (qsizetype i=0;i!=array.size();++i)
    {
        auto jItem = array.at(i).toObject();
        items->append(new ITEM(jItem));
    }

//    qDebug() << "Loaded " << items->size() << " " << name;

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

bool ServiceContext::loadReferences(QDir baseDir, QList<Reference*> * references)
{
    return loadItems(baseDir, "references", references);
}

bool ServiceContext::loadScenes(QDir baseDir, QList<Scene*> * scenes)
{
    return loadItems(baseDir, "scenes", scenes);
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
        auto screenshot = new Screenshot();
        screenshot->filename = entry;
        screenshot->id = QFileInfo(entry).baseName().toInt();
        screenshots->append(screenshot);
    }

    return true;
}

// METHODS TO SAVE THE DATA BACK AS JSON

bool ServiceContext::saveContext(QDir contextFolder, Project * context)
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

bool ServiceContext::saveReferences(QDir contextDir, QList<Reference*> * references)
{
    return saveItems(contextDir, "references", references);
}

bool ServiceContext::saveScenes(QDir contextDir, QList<Scene*> * scenes)
{
    return saveItems(contextDir, "scenes", scenes);
}

bool ServiceContext::saveScreenshots(QDir contextDir, QList<Screenshot*> * screenshots)
{
    if (!contextDir.exists("screenshots") && !contextDir.mkdir("screenshots"))
    {
        qWarning() << "Save error, could not create screenshots folder.";
        return false;
    }

    if  (!contextDir.cd("screenshots"))
    {
        qWarning() << "Save error, could not access the screenshots folder.";
        return false;
    }

    for (auto screenshot : *screenshots)
    {
        QFile file = contextDir.filePath(screenshot->filename);

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


void ContextReport::success(QString const message)
{
//    qInfo() << message;
    _success = true;
    _message = message;
}

void ContextReport::fail(QString const message)
{
//    qWarning() << message;
    _success = false;
    _message = message;
}

bool ContextReport::success() const
{
    return _success;
}

const QString &ContextReport::message() const
{
    return _message;
}

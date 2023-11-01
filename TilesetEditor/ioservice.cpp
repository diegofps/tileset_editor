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

#define SKIP(msg) {\
    qWarning() << (msg);\
    continue;\
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
    // Check tilesets folder
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

    // check masks folder
    QDir masksDir(App::getState()->project()->path);
    char const * masksFoldername = "masks.low";

    if (masksDir.exists(masksFoldername))
    {
        QDir tmp = masksDir;
        if (!tmp.cd(masksFoldername) || !tmp.removeRecursively())
            ABORT("Could not remove previous masks output folder");
    }

    if (!masksDir.mkdir(masksFoldername))
        ABORT("Could not create new masks output folder");

    if (!masksDir.cd(masksFoldername))
        ABORT("Could not access masks output folder");

    // Brushes
    QBrush brushBackground;
    brushBackground.setStyle(Qt::SolidPattern);

    QBrush brushMaskBackground;
    brushMaskBackground.setColor(QColor(0,0,0));
    brushMaskBackground.setStyle(Qt::SolidPattern);

    for (auto tileset : *App::getState()->allTilesets())
    {
        auto state = App::getState();
        auto tileCache = App::getOriginalTileCache();

        QString filename1 = QString("%1_%2.png").arg(tileset->sceneId).arg(tileset->id);
        QString filepath1 = tilesetsDir.filePath(filename1);
        QString filename2 = QString("%1_%2.mask.png").arg(tileset->sceneId).arg(tileset->id);
        QString filepath2 = masksDir.filePath(filename2);
        QFile file1(filepath1);
        QFile file2(filepath2);

        if (file1.exists() && !file1.remove())
            ABORT(QString("Failed to remove previous tileset file: %1").arg(filename1));

        if (file2.exists() && !file2.remove())
            ABORT(QString("Failed to remove previous tileset mask file: %1").arg(filename2));

        QImage img1(tileset->gridW*8, tileset->gridH*8, QImage::Format_ARGB32);
        QImage img2(tileset->gridW*8, tileset->gridH*8, QImage::Format_Grayscale8);

        QPainter painter1(&img1);
        QPainter painter2(&img2);

        brushBackground.setColor(tileset->bgColor);
        painter1.fillRect(img1.rect(), brushBackground);
        painter2.fillRect(img2.rect(), brushMaskBackground);

        for (auto pair : tileset->cells.asKeyValueRange())
        {
            auto cell = pair.second;
            auto tile = state->getTileById(cell->tileID);
            auto palette = state->getPaletteById(cell->paletteID);

            QPixmap * pixmap1 = tileCache->getTilePixmap(tile, palette, cell->hFlip, cell->vFlip);
            QPixmap * pixmap2 = tileCache->getMaskPixmap(tile, palette, cell->hFlip, cell->vFlip);

            if (pixmap1 == nullptr || pixmap2 == nullptr)
                continue;

            painter1.drawPixmap(QRect(cell->x*8, cell->y*8, 8, 8), *pixmap1);
            painter2.drawPixmap(QRect(cell->x*8, cell->y*8, 8, 8), *pixmap2);
        }

        img1.save(filepath1);
        img2.save(filepath2);
    }

    SUCCESS("Build Tilesets completed successfully");
}

void IOService::buildHDTiles(IOReport *report)
{
    // Check the input folder exists
    QDir tilesetsDir(App::getState()->project()->path);
    char const * tilesetsFoldername = "tilesets.high";

    if (!tilesetsDir.exists(tilesetsFoldername))
        ABORT(QString("Could not find %1 folder").arg(tilesetsFoldername));

    if (!tilesetsDir.cd(tilesetsFoldername))
        ABORT(QString("Could not access %1 folder").arg(tilesetsFoldername));

    // Check and create the output folder
    QDir tilesDir(App::getState()->project()->path);
    char const * tilesFoldername = "tiles.high";

    if (tilesDir.exists(tilesFoldername))
    {
        QDir tmp = tilesDir;
        if (!tmp.cd(tilesFoldername) || !tmp.removeRecursively())
            ABORT(QString("Could not remove previous %1 output folder").arg(tilesFoldername));
    }

    if (!tilesDir.mkdir(tilesFoldername))
        ABORT(QString("Could not create new %1 output folder").arg(tilesFoldername));

    if (!tilesDir.cd(tilesFoldername))
        ABORT(QString("Could not access new %1 output folder").arg(tilesFoldername));

    // Iterate over all tilesets and crop their linked cells
    for (auto tileset : *App::getState()->allTilesets())
    {
        QString tsFilename {QString("%1_%2.png").arg(tileset->sceneId).arg(tileset->id)};
        QString tsFilepath {tilesetsDir.filePath(tsFilename)};
        QImage image;

        if (!image.load(tsFilepath))
        {
            qWarning() << QString("Could not load HD tileset from %1").arg(tsFilepath);
            continue;
        }

        if (image.width() % tileset->gridW != 0 || image.height() % tileset->gridH != 0)
        {
            qWarning() << QString("HD tileset has an unexpected size. It should be divisible by %1x%2.")
                          .arg(tileset->gridW)
                          .arg(tileset->gridH);
            continue;
        }

        int const hdCellWidth = image.width() / tileset->gridW;
        int const hdCellHeight = image.height() / tileset->gridH;

        for (auto pair : tileset->cells.asKeyValueRange())
        {
            auto cell = pair.second;
            auto tile = App::getState()->getTileById(cell->tileID);

            if (tile->linkedCellID != cell->id)
                continue;

            QString tFilename {QString("%1_%2_%3_%4.png").arg(tile->id).arg(cell->paletteID).arg(cell->hFlip).arg(cell->vFlip)};
            QString tFilepath {tilesDir.filePath(tFilename)};

            QRect rect(cell->x*hdCellWidth, cell->y*hdCellHeight, hdCellWidth, hdCellHeight);
            image.copy(rect).save(tFilepath);
        }
    }

    SUCCESS("Build HD Tiles completed successfully");
}

inline void scaleUpMask(QImage const & in, QImage & out)
{
    if (in.width() * 2 != out.width() || in.height() * 2 != out.height())
    {
        qWarning() << "Wrong images size for scaleMaskUp, in=" << in.size() << ", out=" << out.size();
        return;
    }

#define LINE_IN(i) in.scanLine((i)>>1);
#define PIXEL_IN(j,line) line[(j)>>1]

    int const j1 = out.width()-2;
    int const j2 = out.width()-1;

    int const i1 = out.height()-2;
    int const i2 = out.height()-1;

    // Top
    {
        auto lineD = LINE_IN(0);
        auto lineG = LINE_IN(1);
        auto lineOut = out.scanLine(0);

        // TopLeft
        {
            int const E = PIXEL_IN(0, lineD);
            int const F = PIXEL_IN(1, lineD);

            int const H = PIXEL_IN(0, lineG);
            int const I = PIXEL_IN(1, lineG);

            lineOut[0] = E+F+H+I>2*255?255:0;
        }

        // TopCenter
        for (int j=1;j!=out.width()-1;++j)
        {
            int const D = PIXEL_IN(j-1, lineD);
            int const E = PIXEL_IN(j  , lineD);
            int const F = PIXEL_IN(j+1, lineD);

            int const G = PIXEL_IN(j-1, lineG);
            int const H = PIXEL_IN(j  , lineG);
            int const I = PIXEL_IN(j+1, lineG);

            lineOut[j] = D+E+F+G+H+I>3*255?255:0;
        }

        // TopRight
        {
            int const D = PIXEL_IN(j1, lineD);
            int const E = PIXEL_IN(j2, lineD);

            int const G = PIXEL_IN(j1, lineG);
            int const H = PIXEL_IN(j2, lineG);

            lineOut[j2] = D+E+G+H>2*255?255:0;
        }
    }

    // Center
    for (int i=1;i!=out.height()-1;++i)
    {
        auto lineA = LINE_IN(i-1);
        auto lineD = LINE_IN(i  );
        auto lineG = LINE_IN(i+1);
        auto lineOut = out.scanLine(i);

        // CenterLeft
        {
            int const B = PIXEL_IN(0, lineA);
            int const C = PIXEL_IN(1, lineA);

            int const E = PIXEL_IN(0, lineD);
            int const F = PIXEL_IN(1, lineD);

            int const H = PIXEL_IN(0, lineG);
            int const I = PIXEL_IN(1, lineG);

            lineOut[0] = B+C+E+F+H+I>3*255?255:0;
        }

        // CenterCenter
        for (int j=1;j!=out.width()-1;++j)
        {
            int const A = PIXEL_IN(j-1, lineA);
            int const B = PIXEL_IN(j  , lineA);
            int const C = PIXEL_IN(j+1, lineA);

            int const D = PIXEL_IN(j-1, lineD);
            int const E = PIXEL_IN(j  , lineD);
            int const F = PIXEL_IN(j+1, lineD);

            int const G = PIXEL_IN(j-1, lineG);
            int const H = PIXEL_IN(j  , lineG);
            int const I = PIXEL_IN(j+1, lineG);

            lineOut[j] = A+B+C+D+E+F+G+H+I>=5*255?255:0;
        }
        // CenterRight
        {
            int const A = PIXEL_IN(j1, lineA);
            int const B = PIXEL_IN(j2, lineA);

            int const D = PIXEL_IN(j1, lineD);
            int const E = PIXEL_IN(j2, lineD);

            int const G = PIXEL_IN(j1, lineG);
            int const H = PIXEL_IN(j2, lineG);

            lineOut[j2] = A+B+D+E+G+H>3*255?255:0;
        }
    }

    // Bottom
    {
        auto lineA = LINE_IN(i1);
        auto lineD = LINE_IN(i2);
        auto lineOut = out.scanLine(i2);

        // BottomLeft
        {
            int const B = PIXEL_IN(0, lineA);
            int const C = PIXEL_IN(1, lineA);

            int const E = PIXEL_IN(0, lineD);
            int const F = PIXEL_IN(1, lineD);

            lineOut[0] = B+C+E+F>2*255?255:0;
        }

        // BottomCenter
        for (int j=1;j!=out.width()-1;++j)
        {
            int const A = PIXEL_IN(j-1, lineA);
            int const B = PIXEL_IN(j  , lineA);
            int const C = PIXEL_IN(j+1, lineA);

            int const D = PIXEL_IN(j-1, lineD);
            int const E = PIXEL_IN(j  , lineD);
            int const F = PIXEL_IN(j+1, lineD);

            lineOut[j] = A+B+C+D+E+F>3*255?255:0;
        }

        // BottomRight
        {
            auto lineA = LINE_IN(i1);
            auto lineD = LINE_IN(i2);
            auto lineOut = out.scanLine(i2);

            int const A = PIXEL_IN(j1, lineA);
            int const B = PIXEL_IN(j2, lineA);

            int const D = PIXEL_IN(j1, lineD);
            int const E = PIXEL_IN(j2, lineD);

            lineOut[j2] = A+B+D+E>2*255?255:0;
        }
    }
}

void IOService::buildEncodedHDTiles(IOReport *report)
{
    auto state = App::getState();

    // Check the input folder exists
    QDir tilesDir(state->project()->path);
    char const * tilesFoldername = "tiles.high";

    if (!tilesDir.exists(tilesFoldername))
        ABORT(QString("Could not find %1 folder").arg(tilesFoldername));

    if (!tilesDir.cd(tilesFoldername))
        ABORT(QString("Could not access %1 folder").arg(tilesFoldername));

    // Check and create the output folder
    QDir encodedTilesDir(state->project()->path);
    char const * encodedTilesFoldername = "tiles.encoded";

    if (encodedTilesDir.exists(encodedTilesFoldername))
    {
        QDir tmp = encodedTilesDir;
        if (!tmp.cd(encodedTilesFoldername) || !tmp.removeRecursively())
            ABORT(QString("Could not remove previous %1 output folder").arg(encodedTilesFoldername));
    }

    if (!encodedTilesDir.mkdir(encodedTilesFoldername))
        ABORT(QString("Could not create new %1 output folder").arg(encodedTilesFoldername));

    if (!encodedTilesDir.cd(encodedTilesFoldername))
        ABORT(QString("Could not access new %1 output folder").arg(encodedTilesFoldername));

    // Encode all tiles
    QByteArray data(128*128*4, '\0');
    QImage mask01( 8*1,  8*1, QImage::Format_Grayscale8);
    QImage mask02( 8*2,  8*2, QImage::Format_Grayscale8);
    QImage mask04( 8*4,  8*4, QImage::Format_Grayscale8);
    QImage mask08( 8*8,  8*8, QImage::Format_Grayscale8);
    QImage mask16(8*16, 8*16, QImage::Format_Grayscale8);

    for (auto & filename : tilesDir.entryList(QDir::Files))
    {
        QString tFilepath { tilesDir.filePath(filename) };
        QFileInfo tInfo(tFilepath);
        bool ok;

        auto ids = tInfo.baseName().split('_');
        if (ids.size() != 4) SKIP(QString("Invalid format in filename: %1.").arg(filename));

        auto tileID = ids[0].toInt(&ok);
        if (!ok) SKIP(QString("Invalid tile id format in filename: %1.").arg(filename));

        auto paletteID = ids[1].toInt(&ok);
        if (!ok) SKIP(QString("Invalid palette id format in filename: %1.").arg(filename));

        bool hFlip = ids[2].toInt(&ok);
        if (!ok) SKIP(QString("Invalid palette id format in filename: %1.").arg(filename));

        bool vFlip = ids[3].toInt(&ok);
        if (!ok) SKIP(QString("Invalid palette id format in filename: %1.").arg(filename));

        auto tile = state->getTileById(tileID);
        if (tile == nullptr) SKIP(QString("Invalid tile id=%1 in filename=").arg(tileID).arg(filename));

        auto palette = state->getPaletteById(paletteID);
        if (palette == nullptr) SKIP(QString("Invalid palette id=%1 in filename=").arg(paletteID).arg(filename));

        QImage hdImg;
        if (!hdImg.load(tFilepath)) SKIP(QString("Failed to load HD tile from filename %1.").arg(filename));

        if (hdImg.width() != 128) SKIP(QString("Invalid input tile in %1. A tile width must be equal to 128.").arg(filename));
        if (hdImg.height() != 128) SKIP(QString("Invalid input tile in %1. A tile height must be equal to 128.").arg(filename));

        int const gridWidth = hdImg.width() / 8;
        int const gridHeight = hdImg.height() / 8;

        QFile eFile {encodedTilesDir.filePath(filename)};
        if (!eFile.open(QIODevice::WriteOnly)) SKIP(QString("Encode error, could not open the output file: %1").arg(eFile.fileName()));

        // TODO: Build transparency mask mapping the original tile to HD resolution. Apply a convolution to smooth it
        App::getOriginalTileCache()->loadMask(tile, hFlip, vFlip, mask01);
        scaleUpMask(mask01, mask02);
        scaleUpMask(mask02, mask04);
        scaleUpMask(mask04, mask08);
        scaleUpMask(mask08, mask16);

        mask01.save("./mask01.png");
        mask02.save("./mask02.png");
        mask04.save("./mask04.png");
        mask08.save("./mask08.png");
        mask16.save("./mask16.png");

        qInfo() << "Masks exported";
        SUCCESS("Encoded HD Tiles finished successfully");

        return;

        int k = 0;
        for (int i=0;i!=hdImg.height();++i)
        {
            int const y = i / gridHeight;

            QRgb const * lineHD = reinterpret_cast<QRgb const*>(hdImg.constScanLine(i));

            for (int j=0;j!=hdImg.width();++j)
            {
                int const x = j / gridWidth;
                int const pseudoPixel = y*8+x;

                auto pseudoColor = tile->pixels[pseudoPixel];

//                img.pixel()
                // TODO: Use the transparency mask
                // TODO: Check if colorIndex is within the palette size
                // TODO: Save the original pseudoColor and the offset for the HD tile

                QColor color = palette->colors[pseudoColor];

                int const r = 0;

                data[k] = pseudoColor; ++k;
            }
        }
    }
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

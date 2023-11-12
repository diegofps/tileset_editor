#include "tilecachehd.h"
#include "app.h"
#include <QFile>
#include <QDir>
#include <QPainter>

#include "helpers/vectors.h"
#include "helpers/recolor.h"

TileCacheHD::TileCacheHD()
{

}

void TileCacheHD::clear()
{
    for (auto pair : _cachedImages.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedPixmaps.asKeyValueRange())
        delete pair.second;

    for (auto pair : _tileID2tileFile.asKeyValueRange())
        delete pair.second;

    _cachedImages.clear();
    _cachedPixmaps.clear();
    _tileID2tileFile.clear();
}

void TileCacheHD::updateKnownTiles(QString const & projectPath)
{
#define ABORT(msg) {\
        qWarning() << "Could not parse HD Tiles: " << msg;\
        return;\
    }

#define SKIP(msg) {\
        qWarning() << "Could not parse HD Tile: " << msg;\
        continue;\
    }

    QDir tilesDir(projectPath);
    char const * tilesFoldername = "tiles.high";

    if (!tilesDir.exists(tilesFoldername)) ABORT("Tiles folder does not exists");
    if (!tilesDir.cd(tilesFoldername)) ABORT("Could not cd to tiles folder");

    bool ok;

    for (auto & filename : tilesDir.entryList(QDir::Files))
    {
        auto basename = QFileInfo(filename).baseName();
        auto cells = basename.split("_");

        if (cells.size() != 5)
        {
            qWarning() << "Invalid tile file:" << basename;
            continue;
        }

        int32_t tileID = cells[0].toInt(&ok);
        if (!ok) SKIP(QString("Invalid tileID value in filename: %1.").arg(filename));

        int32_t paletteID = cells[1].toInt(&ok);
        if (!ok) SKIP(QString("Invalid paletteID value in filename: %1.").arg(filename));

        int hFlip = cells[2].toInt(&ok);
        if (!ok) SKIP(QString("Invalid hFlip value in filename: %1.").arg(filename));

        int vFlip = cells[3].toInt(&ok);
        if (!ok) SKIP(QString("Invalid vFlip value in filename: %1.").arg(filename));

        int tilesetID = cells[4].toInt(&ok);
        if (!ok) SKIP(QString("Invalid tilesetID value in filename: %1.").arg(filename));

        auto tileFile = new TileFile();
        tileFile->filename = filename;
        tileFile->tileID = tileID;
        tileFile->paletteID = paletteID;
        tileFile->tilesetID = tilesetID;
        tileFile->hFlip = hFlip;
        tileFile->vFlip = vFlip;
        _tileID2tileFile.emplace(tileID, tileFile);
    }
    qWarning() << "Found " << _tileID2tileFile.size() << " tiles";
#undef ABORT
#undef SKIP
}

QImage * TileCacheHD::loadImage(Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
#define ABORT(msg) {\
        qWarning() << "Could not load HD Tile: " << msg;\
        return nullptr;\
    }

    auto project = App::getState()->project();
    if (project == nullptr) ABORT("Project is nullptr");

    QDir tilesDir(project->path);
    char const * tilesFoldername = "tiles.high";

    if (!tilesDir.exists(tilesFoldername)) ABORT("Tiles folder does not exists");
    if (!tilesDir.cd(tilesFoldername)) ABORT("Could not cd to tiles folder");

    QDir masksDir(project->path);
    char const * masksFoldername = "masks.high";

    if (!masksDir.exists(masksFoldername)) ABORT("Masks folder does not exists");
    if (!masksDir.cd(masksFoldername)) ABORT("Could not cd to masks folder");

    auto it = _tileID2tileFile.constFind(tile->id);
    if (it == _tileID2tileFile.constEnd()) ABORT(QString("Could not find tileFile for tileID=%1").arg(tile->id));

    auto tileFile = it.value();
    auto paletteOld = App::getState()->getPaletteById(tileFile->paletteID);
    if (paletteOld == nullptr) ABORT("Could not find the original palette");

    QImage imgIn;
    if (!imgIn.load(tilesDir.filePath(tileFile->filename))) ABORT(QString("Could not find tile file for tileID=%1").arg(tile->id));

    QImage maskIn;
    if (!maskIn.load(masksDir.filePath(tileFile->filename))) ABORT(QString("Could not find mask file for tileID=%1").arg(tile->id));

    QImage * imgOut = new QImage(imgIn.width(), imgIn.height(), QImage::Format_ARGB32);

    QImageWrapper inputWrapper(imgIn, false, false);
    QImageWrapper maskWrapper(maskIn, false, false);
    QImageWrapper outputWrapper(*imgOut, hFlip ^ tileFile->hFlip, vFlip ^ tileFile->vFlip);

    std::vector<Vector3F> colors1;
    std::vector<Vector3F> colors2;

    for (qsizetype i=1;i!=paletteOld->size;++i)
    {
        QColor & c = paletteOld->colors[i];
        colors1.push_back(Vector3F(c.red(), c.green(), c.blue()));
    }

    for (qsizetype i=1;i!=palette->size;++i)
    {
        QColor & c = palette->colors[i];
        colors2.push_back(Vector3F(c.red(), c.green(), c.blue()));
    }

    if (tile->id == 4070)
        qWarning() << "Found it!";

    recolor12(colors1, colors2, inputWrapper, maskWrapper, outputWrapper);

//    qWarning() << "Loaded tile for tileID=" << tile->id << " paletteID=" << palette->id << " hFlip=" << hFlip << " vFlip=" << vFlip;

    return imgOut;
#undef ABORT
}

void TileCacheHD::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                              Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage * img = loadImage(tile, palette, hFlip, vFlip);

    _cachedImages[key] = img == nullptr ? nullptr : img;
    _cachedPixmaps[key] = img == nullptr ? nullptr : new QPixmap(QPixmap::fromImage(*img));
}

QPixmap * TileCacheHD::getTilePixmap(Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedPixmaps.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedPixmaps[key];
}

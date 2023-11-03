#include "tilecachehd.h"
#include "app.h"
#include <QFile>
#include <QDir>
#include <QPainter>

TileCacheHD::TileCacheHD()
{

}

void TileCacheHD::clear()
{
    for (auto pair : _cachedImages.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedPixmaps.asKeyValueRange())
        delete pair.second;

    _cachedImages.clear();
    _cachedPixmaps.clear();
}

void TileCacheHD::loadImage(Tile * tile, Palette * palette, bool hFlip, bool vFlip, QImage & img)
{
#define ABORT(msg) {\
        qWarning() << "Could not load HD Tile: " << msg;\
        QPainter painter(&img);\
        painter.fillRect(img.rect(), QColor::fromString("#000000"));\
        return;\
    }

    if (img.width() != 128 || img.height() != 128 || img.format() != QImage::Format_ARGB32)
        ABORT("Invalid image format or size for TileCacheHD::loadImage. Expected 128x128 Format_ARGB32.");

    auto project = App::getState()->project();
    if (project == nullptr) ABORT("Project is nullptr");

    QDir encodedDir(project->path);
    char const * encodedFoldername = "encoded.high";

    if (!encodedDir.exists(encodedFoldername)) ABORT("Encoded folder does not exists");
    if (!encodedDir.cd(encodedFoldername)) ABORT("Could not cd to encoded folder");

    QString filename {QString("%1_%3_%4.bin").arg(tile->id).arg(hFlip).arg(vFlip)};
    QFile file(encodedDir.filePath(filename));
    if (!file.open(QIODevice::ReadOnly)) ABORT(QString("File %1 could not be open.").arg(filename));

    QByteArray data = file.readAll();

    if (data.size() != 128*128*4) ABORT(QString("File %1 has an invalid size.").arg(filename));

    auto srcColors = palette->colors;
    int i0,i1,iStep;
    int j0,j1,jStep;
    int k = 0;

//    int32_t const originalPaletteID = *((int32_t*)data.data());
//    auto originalPalette = App::getState()->getPaletteById(originalPaletteID);
//    if (!encodedDir.cd(encodedFoldername)) ABORT("Could not load original palette");
//    k += 4;

    if (vFlip) { i0 = 127; i1 = -1  ; iStep = -1; }
    else       { i0 = 0  ; i1 = +128; iStep = +1; }

    if (hFlip) { j0 = 127; j1 = -1  ; jStep = -1; }
    else       { j0 = 0  ; j1 = +128; jStep = +1; }

    QRgb newColor;

    for (int i=i0;i!=i1;i+=iStep)
    {
        for (int j=j0;j!=j1;j+=jStep)
        {
            int const idx = data[k]; ++k;
            int const ri = data[k]; ++k;
            int const gi = data[k]; ++k;
            int const bi = data[k]; ++k;

            if (idx == 0)
            {
                newColor = qRgba(0,0,0,0);
            }
            else if (idx > palette->size)
            {
                qWarning() << "Color index is higher then palette size: idx=" << idx << ", size=" << palette->size;
                newColor = qRgba(255,0,0,255);
            }
            else
            {
                QColor & color = srcColors[idx];

                newColor = qRgba(
                            decodeHDPixel(ri,color.red()),
                            decodeHDPixel(gi,color.green()),
                            decodeHDPixel(bi,color.blue()),
                            255);

//                qWarning() << ri << gi << bi <<
//                              "\n  " << color.red() << color.green() << color.blue() << color.alpha() <<
//                              "\n  " << qRed(newColor) << qGreen(newColor) << qBlue(newColor) << qAlpha(newColor) <<
//                              "\n";
            }

//            qWarning() << qRed(newColor) << qGreen(newColor) << qBlue(newColor) << qAlpha(newColor);
            img.setPixel(j, i, newColor);
        }
    }

}

void TileCacheHD::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                              Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage img(128,128, QImage::Format_ARGB32);

    loadImage(tile, palette, hFlip, vFlip, img);

    _cachedImages[key] = new QImage(img);
    _cachedPixmaps[key] = new QPixmap(QPixmap::fromImage(img));
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

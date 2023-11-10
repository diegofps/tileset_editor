#include "tilecache.h"

TileCache::TileCache()
{

}

void TileCache::clear()
{
    for (auto pair : _cachedPixmaps.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedImages.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedMaskPixmaps.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedMaskImages.asKeyValueRange())
        delete pair.second;

    _cachedImages.clear();
    _cachedPixmaps.clear();
    _cachedMaskImages.clear();
    _cachedMaskPixmaps.clear();
}

void TileCache::loadMask(Tile * tile, bool hFlip, bool vFlip, QImage & img)
{
    if (img.width() != 8 || img.height() != 8 || img.format() != QImage::Format_Grayscale8)
    {
        qWarning() << "Invalid image format or size for TileCache::loadMask. Expected 8x8 Format_Grayscale8.";
        return;
    }

    auto srcPixels = tile->pixels;
    int i0,i1,iStep;
    int j0,j1,jStep;
    int k = 0;

    if (vFlip) { i0 = 7; i1 = -1; iStep = -1; }
    else       { i0 = 0; i1 = +8; iStep = +1; }

    if (hFlip) { j0 = 7; j1 = -1; jStep = -1; }
    else       { j0 = 0; j1 = +8; jStep = +1; }

    for (int i=i0;i!=i1;i+=iStep)
    {
        uchar * line = img.scanLine(i);
        for (int j=j0;j!=j1;j+=jStep)
            line[j] = srcPixels[k++] == 0 ? 0 : 255;
    }
}

void TileCache::loadImage(Tile * tile, Palette * palette, bool hFlip, bool vFlip, QImage & img)
{
    if (img.width() != 8 || img.height() != 8 || img.format() != QImage::Format_ARGB32)
    {
        qWarning() << "Invalid image format or size for TileCache::loadImage. Expected 8x8 Format_ARGB32.";
        return;
    }

    auto srcColors = palette->colors;
    auto srcPixels = tile->pixels;
    int i0,i1,iStep;
    int j0,j1,jStep;
    int k = 0;

    if (vFlip) { i0 = 7; i1 = -1; iStep = -1; }
    else       { i0 = 0; i1 = +8; iStep = +1; }

    if (hFlip) { j0 = 7; j1 = -1; jStep = -1; }
    else       { j0 = 0; j1 = +8; jStep = +1; }

    for (int i=i0;i!=i1;i+=iStep)
    {
        QRgb * line = reinterpret_cast<QRgb*>(img.scanLine(i));
        for (int j=j0;j!=j1;j+=jStep)
        {
            auto & idx = srcPixels[k];

            if (idx == 0)
            {
                line[j] = qRgba(0, 0, 0, 0);
            }
            else if (idx >=palette->size)
            {
                qWarning() << "Color index is higher then palette size: idx=" << idx << ", size=" << palette->size;
                line[j] = qRgba(255, 0, 0, 255);
            }
            else
            {
                QColor & color = srcColors[idx];
                line[j] = qRgba(color.red(), color.green(), color.blue(), 255);
            }

            ++k;
        }
    }
}

void TileCache::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                            Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage *img = new QImage(8,8, QImage::Format_ARGB32);
    QImage *mask = new QImage(8,8, QImage::Format_Grayscale8);

    loadImage(tile, palette, hFlip, vFlip, *img);
    loadMask(tile, hFlip, vFlip, *mask);

    _cachedImages[key] = img;
    _cachedPixmaps[key] = new QPixmap(QPixmap::fromImage(*img));
    _cachedMaskImages[key] = mask;
    _cachedMaskPixmaps[key] = new QPixmap(QPixmap::fromImage(*mask));
}

QImage * TileCache::getTileImage(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedImages.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedImages[key];
}

QImage * TileCache::getMaskImage(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedMaskImages.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedMaskImages[key];
}

QPixmap * TileCache::getTilePixmap(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedPixmaps.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedPixmaps[key];
}

QPixmap * TileCache::getMaskPixmap(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedMaskPixmaps.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedMaskPixmaps[key];
}

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

    _cachedImages.clear();
    _cachedPixmaps.clear();
}

void TileCache::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                            Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage img(8,8, QImage::Format_ARGB32);
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

    _cachedImages[key] = new QImage(img);
    _cachedPixmaps[key] = new QPixmap(QPixmap::fromImage(img));
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

QPixmap * TileCache::getTilePixmap(Tile * tile, Palette * palette, bool const hFlip, bool const vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedPixmaps.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedPixmaps[key];
}

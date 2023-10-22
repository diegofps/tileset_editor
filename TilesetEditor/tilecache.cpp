#include "tilecache.h"

TileCache::TileCache()
{

}

void TileCache::clear()
{
    for (auto pair : _cache.asKeyValueRange())
        delete pair.second;
    _cache.clear();
}

QPixmap * TileCache::getTilePixmap(Tile * tile, Palette * palette)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<int,int> key(tile->id, palette->id);

    if (!_cache.contains(key))
    {
        QImage img(8,8, QImage::Format_ARGB32);
        auto srcColors = palette->colors;
        auto srcPixels = tile->pixels;
        int k = 0;

        for (int i=0;i!=8;++i)
        {
            QRgb * line = reinterpret_cast<QRgb*>(img.scanLine(i));
            for (int j=0;j!=8;++j)
            {
                auto & idx = srcPixels[k];

                if (idx >=palette->size)
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

        QPixmap * pixmap = new QPixmap(QPixmap::fromImage(img));
        _cache[key] = pixmap;

    }

    return _cache[key];
}

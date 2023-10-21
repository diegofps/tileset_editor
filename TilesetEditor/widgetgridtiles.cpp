#include "app.h"
#include "widgetgridtiles.h"

#include <QResizeEvent>
#include <QPainter>

#define TILE_SIZE 32
#define SCROLL_SPACE 0
#define SEPARATOR_SPACE 2


WidgetGridTiles::WidgetGridTiles(QWidget *parent)
    : QWidget{parent},

      _cols(0),
      _rows(0)
{
    setMinimumHeight(100);
    _pen.setColor(QColor::fromRgb(0,255,0));
    _brush.setColor(QColor::fromRgb(255,0,255));
    _brush.setStyle(Qt::SolidPattern);
}

void WidgetGridTiles::clear()
{
    _tiles.clear();
}

void WidgetGridTiles::append(Tile * value)
{
    _tiles.append(value);
}

void WidgetGridTiles::repack()
{
    _cols = width() <= SCROLL_SPACE + TILE_SIZE ? 1 : (width() - SCROLL_SPACE - TILE_SIZE) / (TILE_SIZE + SEPARATOR_SPACE) + 1;
    _rows = _tiles.empty() ? 1 : ceil(_tiles.size() / float(_cols));

    int const expectedHeight = _rows * TILE_SIZE + (_rows-1) * SEPARATOR_SPACE;

    if (expectedHeight != height())
        setMinimumHeight(expectedHeight);
}

void WidgetGridTiles::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    for (int i=0;i!=_tiles.size();++i)
    {
        Tile * tile = _tiles[i];
        Palette * palette = App::getState()->getProjectPaletteById(tile->preferredPalette);
        QPixmap * img = App::getOriginalTileCache()->getTilePixmap(tile, palette);

        if (img != nullptr)
        {
            int const gridX = i % _cols;
            int const gridY = i / _cols;

            int const x = gridX * TILE_SIZE + (gridX-1) * SEPARATOR_SPACE;
            int const y = gridY * TILE_SIZE + (gridY-1) * SEPARATOR_SPACE;

            QRect pos(x,y,TILE_SIZE,TILE_SIZE);
            painter.drawPixmap(pos, *img);
        }
    }
}

void WidgetGridTiles::resizeEvent(QResizeEvent *event)
{
    repack();
}

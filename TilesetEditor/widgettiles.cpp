#include "app.h"
#include "widgettiles.h"

#include <QResizeEvent>
#include <QPainter>

#define TILE_SIZE 32
#define SCROLL_SPACE 0
#define SEPARATOR_SPACE 2

WidgetTiles::WidgetTiles(QWidget *parent)
    : QWidget{parent},

      _cols(0),
      _rows(0),
      _selection(-1,-1)
{
    setMinimumHeight(100);
//    setMouseTracking(true);

    _pen.setColor(QColor::fromRgb(255,0,0));
    _pen.setStyle(Qt::SolidLine);
    _pen.setWidth(3);

    _penLinkRequired.setColor(QColor::fromString("#66ffff00"));
    _penLinkRequired.setStyle(Qt::SolidLine);
    _penLinkRequired.setWidth(1);

    _brush.setColor(QColor::fromRgba(qRgba(128,128,255,160)));
    _brush.setStyle(Qt::SolidPattern);
}

void WidgetTiles::setTiles(QList<Tile*> const * value)
{
    _tiles.clear();
    for (auto t : *value)
        _tiles.append(t);

    _views.resize(value->size());
    for (qsizetype i=0;i!=value->size();++i)
    {
        auto & view = _views[i];
        auto tile   = _tiles[i];
        Palette * palette = App::getState()->getPaletteById(tile->favoritePaletteID);

        view.i = 0;
        view.j = 0;
        view.tile = tile;
        view.pixmap = App::getOriginalTileCache()->getTilePixmap(tile, palette, false, false);
        view.rect = QRect();
    }

    _selection.set(-1,-1);

    update();
}

void WidgetTiles::repack()
{
    _cols = width() <= SCROLL_SPACE + TILE_SIZE ? 1 : (width() - SCROLL_SPACE - TILE_SIZE) / (TILE_SIZE + SEPARATOR_SPACE) + 1;
    _rows = _tiles.empty() ? 1 : ceil(_tiles.size() / float(_cols));

    int const expectedHeight = _rows * TILE_SIZE + (_rows-1) * SEPARATOR_SPACE;

    if (expectedHeight != height())
        setMinimumHeight(expectedHeight);

    for (int i=0;i!=_tiles.size();++i)
    {
        ViewHolder * view = &_views[i];

        view->j = i % _cols;
        view->i = i / _cols;

        int const x = view->j * TILE_SIZE + (view->j-1) * SEPARATOR_SPACE;
        int const y = view->i * TILE_SIZE + (view->i-1) * SEPARATOR_SPACE;

        view->rect = QRect(x,y,TILE_SIZE,TILE_SIZE);
    }

    update();
}

void WidgetTiles::moveTileSelection(int rx, int ry)
{
    int const newStart = _selection.start + _cols * ry + rx;
    int const newEnd = _selection.end + _cols * ry + rx;

    if (newStart >= 0 && newStart < _tiles.size() && newEnd >= 0 && newEnd < _tiles.size())
    {
        _selection.set(newStart, newEnd);
        emit onSelectedTileChanged(_selection);
    }

    update();
}

void WidgetTiles::setSelection(Range range)
{
    range.limit(-1, _tiles.size()-1);
    _selection = range;
    update();

    emit onSelectedTileChanged(_selection);
}

void WidgetTiles::paintEvent(QPaintEvent * event)
{
    (void) event;
    QPainter painter(this);
    for (int i=0;i!=_views.size();++i)
    {
        auto & view = _views[i];

        if (view.pixmap != nullptr)
            painter.drawPixmap(view.rect, *view.pixmap);

        if (i >= _selection.start && i <= _selection.end)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(_brush);
            painter.drawRect(view.rect);

            if (i == _selection.start)
                painter.drawRect(view.rect);
        }

        if (view.tile->linkedCellID == 0)
        {
            painter.setPen(_penLinkRequired);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(view.rect);
        }
    }
}

void WidgetTiles::resizeEvent(QResizeEvent *event)
{
    (void) event;
    repack();
}

void WidgetTiles::mousePressEvent(QMouseEvent *event)
{
    int btns = event->buttons();

    if (!(btns & Qt::LeftButton))
        return;

    auto pos = event->pos();
    bool mustEmit = false;

    if (event->modifiers().testFlag(Qt::ShiftModifier))
    {
        for (qsizetype i=0;i!=_views.size();++i)
        {
            auto & view = _views[i];

            if (view.rect.contains(pos))
            {
                _selection.end = i;
                mustEmit = true;
                break;
            }
        }
    }
    else
    {
        for (qsizetype i=0;i!=_views.size();++i)
        {
            auto & view = _views[i];

            if (view.rect.contains(pos))
            {
                _selection.start = i;
                _selection.end = i;
                mustEmit = true;
                break;
            }
        }
    }

    if (_selection.start > _selection.end)
    {
        int tmp = _selection.start;
        _selection.start = _selection.end;
        _selection.end = tmp;
    }

    if (mustEmit)
        emit onSelectedTileChanged(_selection);

    update();
}

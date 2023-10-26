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
      _selectionStart(-1),
      _selectionEnd(-1)
{
    setMinimumHeight(100);
//    setMouseTracking(true);

    _pen.setColor(QColor::fromRgb(255,0,0));
    _pen.setStyle(Qt::SolidLine);
    _pen.setWidth(3);

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
        Palette * palette = App::getState()->getProjectPaletteById(tile->preferredPalette);

        view.i = 0;
        view.j = 0;
        view.tile = tile;
        view.pixmap = App::getOriginalTileCache()->getTilePixmap(tile, palette, false, false);
        view.rect = QRect();
    }

    _selectionStart = -1;
    _selectionEnd = -1;
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
}

void WidgetTiles::moveToTile(int rx, int ry)
{
    int const newStart = _selectionStart + _cols * ry + rx;
    int const newEnd = _selectionEnd + _cols * ry + rx;

    if (newStart >= 0 && newStart < _tiles.size() && newEnd >= 0 && newEnd < _tiles.size())
    {
        _selectionStart = newStart;
        _selectionEnd = newEnd;
        emit onSelectedTileChanged(_selectionStart, _selectionEnd);
    }
    update();
}

void WidgetTiles::setSelection(int start, int end)
{
    start = std::max(-1, start);
    end = std::min((int)_tiles.size()-1, end);

    _selectionStart = start;
    _selectionEnd = end;
    emit onSelectedTileChanged(_selectionStart, _selectionEnd);
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

        if (i >= _selectionStart && i <= _selectionEnd)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(_brush);
            painter.drawRect(view.rect);

            if (i == _selectionStart)
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
                _selectionEnd = i;
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
                _selectionStart = i;
                _selectionEnd = i;
                mustEmit = true;
                break;
            }
        }
    }

    if (_selectionStart > _selectionEnd)
    {
        int tmp = _selectionStart;
        _selectionStart = _selectionEnd;
        _selectionEnd = tmp;
    }

    if (mustEmit)
        emit onSelectedTileChanged(_selectionStart, _selectionEnd);

    update();
}

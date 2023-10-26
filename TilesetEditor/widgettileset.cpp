#include "widgettileset.h"

WidgetTileset::WidgetTileset(QWidget *parent)
    : QWidget{parent},

    _root(0,0,8,8),
    _offset(0,0,8,8),
    _viewport(0,0,0,0),
    _viewportPower(7),
    _gridWidth(0),
    _gridHeight(0),
    _cells(nullptr)
{
    _brushRoot.setColor(QColor::fromString("#66ffffff"));
    _brushRoot.setStyle(Qt::SolidPattern);

    _brushOffset.setColor(QColor::fromString("#660000ff"));
    _brushOffset.setStyle(Qt::SolidPattern);

    updateViewport();
    update();
}

void WidgetTileset::paintEvent(QPaintEvent *event)
{
    // TODO
}

void WidgetTileset::resizeEvent(QResizeEvent *event)
{
    updateViewport();
}

void WidgetTileset::setGridSize(int w, int h)
{
    _gridWidth = w;
    _gridHeight = h;
    update();
}

void WidgetTileset::setRoot(int x, int y)
{
    int const offX = _offset.x() - _root.x();
    int const offY = _offset.y() - _root.y();

    _root.setRect(x, y, 8, 8);
    _offset.setRect(x+offX, y+offY, 8, 8);

    update();
}

void WidgetTileset::setOffset(int rx, int ry)
{
    _offset.setRect(_root.x()+rx, _root.y()+ry, 8, 8);
    update();
}

void WidgetTileset::setCells(QHash<QPair<int, int>, Cell *> const * cells)
{
    _cells = cells;
    update();
}

void WidgetTileset::moveViewport(int rx, int ry)
{
    // TODO
}

void WidgetTileset::updateViewport()
{
    int const centerX = _viewport.x() + _viewport.width() / 2;
    int const centerY = _viewport.y() + _viewport.height() / 2;

    int const w = std::pow(1.4, _viewportPower);
    int const h = w * height() / width();

    int const x = centerX - w / 2;
    int const y = centerY - h / 2;

    _viewport.setRect(x, y, w, h);
}


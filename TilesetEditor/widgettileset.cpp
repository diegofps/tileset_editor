#include "widgettileset.h"
#include <QPainter>

WidgetTileset::WidgetTileset(QWidget *parent)
    : QWidget{parent},

    _root(0,0,8,8),
    _offset(0,0,8,8),
    _viewport(0,0,0,0),
    _viewportPower(15),
    _gridWidth(0),
    _gridHeight(0),
    _cells(nullptr)
{
    _brushRoot.setColor(QColor::fromString("#66ffffff"));
    _brushRoot.setStyle(Qt::SolidPattern);

    _brushOffset.setColor(QColor::fromString("#660000ff"));
    _brushOffset.setStyle(Qt::SolidPattern);

    _penGrid.setColor(QColor::fromString("#66ffffff"));
    _penGrid.setStyle(Qt::SolidLine);

    _brushBackground.setColor(QColor::fromString("#ff000000"));
    _brushBackground.setStyle(Qt::SolidPattern);

    updateViewport();
    update();
}

inline void
drawRectangleInViewport(QRect const & rect,
                        QSize const & painterSize,
                        QRect const & viewport,
                        QBrush const & brush,
                        QPen const & pen,
                        QPainter & painter)
{
#define REPROJECT(x,w,vw) (vw==0?0:(x)*(w)/(vw))

    int const x1 = REPROJECT(rect.x()-viewport.x(), painterSize.width(), viewport.width());
    int const y1 = REPROJECT(rect.y()-viewport.y(), painterSize.height(), viewport.height());
    int const x2 = REPROJECT(rect.x()+rect.width()-viewport.x(), painterSize.width(), viewport.width());
    int const y2 = REPROJECT(rect.y()+rect.height()-viewport.y(), painterSize.height(), viewport.height());

    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(QRect(x1,y1,x2-x1,y2-y1));
}

void WidgetTileset::paintEvent(QPaintEvent * event)
{
    (void)event;
    QPainter painter(this);

    painter.fillRect(rect(), _brushBackground);

    drawRectangleInViewport(QRect(0,0,_gridWidth*8, _gridHeight*8), size(), _viewport, Qt::NoBrush, _penGrid, painter);
    drawRectangleInViewport(_root, size(), _viewport, _brushRoot, Qt::NoPen, painter);
    drawRectangleInViewport(_offset, size(), _viewport, _brushOffset, Qt::NoPen, painter);
}

void WidgetTileset::resizeEvent(QResizeEvent * event)
{
    (void)event;
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

    _root.setRect(x*8, y*8, 8, 8);
    _offset.setRect(_root.x()+offX, _root.y()+offY, 8, 8);

    update();
}

void WidgetTileset::setOffset(int rx, int ry)
{
    _offset.setRect(_root.x()+rx*8, _root.y()+ry*8, 8, 8);
    update();
}

void WidgetTileset::setCells(QHash<QPair<int, int>, Cell *> const * cells)
{
    _cells = cells;
    update();
}

void WidgetTileset::setBackgroundColor(QColor value)
{
    int r = (value.red() + 128) % 256;
    int g = (value.green() + 128) % 256;
    int b = (value.blue() + 128) % 256;
    int a = 128;

    _brushBackground.setColor(value);
    _penGrid.setColor(QColor::fromRgb(r,g,b,a));
    update();
}

void WidgetTileset::moveViewport(int rx, int ry)
{
    int const step = std::max(1.0, std::min(_viewport.width(), _viewport.height()) * 0.1);

    int const x = _viewport.x() + rx * step;
    int const y = _viewport.y() + ry * step;
    int const w = _viewport.width();
    int const h = _viewport.height();

    _viewport.setRect(x,y,w,h);
    update();
}

void WidgetTileset::moveViewportHome()
{
    int const x = 0;
    int const y = 0;
    int const w = _viewport.width();
    int const h = _viewport.height();

    _viewport.setRect(x,y,w,h);
    update();
}

void WidgetTileset::setZoom(int value)
{
    _viewportPower = value;
    updateViewport();
    update();
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


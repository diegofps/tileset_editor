#include "widgeteditor.h"
#include <QPainter>
#include <QMouseEvent>
#include "app.h"

WidgetEditor::WidgetEditor(QWidget *parent)
    : QWidget{parent},

    _root(0,0,1,1),
    _offset(0,0,1,1),
    _viewport(0,0,0,0),
    _viewportPower(15),
    _scrollPosition(0),
    _gridWidth(0),
    _gridHeight(0),
    _cells(nullptr),
    _lastHoverKey(-1,-1)
{
    _brushRoot.setColor(QColor::fromString("#66ffffff"));
    _brushRoot.setStyle(Qt::SolidPattern);

    _brushOffset.setColor(QColor::fromString("#660000ff"));
    _brushOffset.setStyle(Qt::SolidPattern);

    _brushLink.setColor(QColor::fromString("#66ffff00"));
    _brushLink.setStyle(Qt::SolidPattern);

    _penLinkRequired.setColor(QColor::fromString("#66ffff00"));
    _penLinkRequired.setStyle(Qt::SolidLine);
    _penLinkRequired.setWidth(1);

    _penGrid.setColor(QColor::fromString("#66ffffff"));
    _penGrid.setStyle(Qt::SolidLine);

    _brushBackground.setColor(QColor::fromString("#ff000000"));
    _brushBackground.setStyle(Qt::SolidPattern);

    setMouseTracking(true);
    updateViewport();
    update();
}

#define REPROJECT(x,w,vw) (vw==0?0:(x)*(w)/(vw))

inline void
drawRectangleInViewport(QRect const & rect,
                        QSize const & painterSize,
                        QRectF const & viewport,
                        QBrush const & brush,
                        QPen const & pen,
                        QPainter & painter)
{
    int const x1 = REPROJECT(rect.x()-viewport.x(), painterSize.width(), viewport.width());
    int const y1 = REPROJECT(rect.y()-viewport.y(), painterSize.height(), viewport.height());
    int const x2 = REPROJECT(rect.x()+rect.width()-viewport.x(), painterSize.width(), viewport.width());
    int const y2 = REPROJECT(rect.y()+rect.height()-viewport.y(), painterSize.height(), viewport.height());

    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(QRect(x1,y1,x2-x1,y2-y1));
}

inline void
drawPixmapInViewport(QRect const & rect,
                     QSize const & painterSize,
                     QRectF const & viewport,
                     QPixmap const & pixmap,
                     QPainter & painter)
{
    int const x1 = REPROJECT(rect.x()-viewport.x(), painterSize.width(), viewport.width());
    int const y1 = REPROJECT(rect.y()-viewport.y(), painterSize.height(), viewport.height());
    int const x2 = REPROJECT(rect.x()+rect.width()-viewport.x(), painterSize.width(), viewport.width());
    int const y2 = REPROJECT(rect.y()+rect.height()-viewport.y(), painterSize.height(), viewport.height());

    painter.drawPixmap(QRect(x1,y1,x2-x1,y2-y1), pixmap);
}

void WidgetEditor::paintEvent(QPaintEvent * event)
{
    (void)event;
    QPainter painter(this);

    // Draw background color
    painter.fillRect(rect(), _brushBackground);

    // Draw grid box
    drawRectangleInViewport(QRect(0,0,_gridWidth*8, _gridHeight*8), size(), _viewport, Qt::NoBrush, _penGrid, painter);

    // Draw cells
    if (_cells != nullptr)
    {
        for (auto pair: _cells->asKeyValueRange())
        {
            auto cell = pair.second;
            auto tile = App::getState()->getTileById(cell->tileID);
            auto palette = App::getState()->getPaletteById(cell->paletteID);
            auto pixmap = App::getOriginalTileCache()->getTilePixmap(tile, palette, cell->hFlip, cell->vFlip);
            QRect cellRect(cell->x*8, cell->y*8, 8, 8);

            drawPixmapInViewport(cellRect, size(), _viewport, *pixmap, painter);

            if (tile->linkedCellID == 0)
                drawRectangleInViewport(cellRect, size(), _viewport, Qt::NoBrush, _penLinkRequired, painter);

            else if (tile->linkedCellID == cell->id)
                drawRectangleInViewport(cellRect, size(), _viewport, _brushLink, Qt::NoPen, painter);

        }
    }

    // Draw root and offset
    drawRectangleInViewport(QRect(_root.x()*8, _root.y()*8, 8, 8), size(), _viewport, _brushRoot, Qt::NoPen, painter);
    drawRectangleInViewport(QRect(_offset.x()*8, _offset.y()*8, 8, 8), size(), _viewport, _brushOffset, Qt::NoPen, painter);
}

void WidgetEditor::resizeEvent(QResizeEvent * event)
{
    (void)event;
    updateViewport();
}

void WidgetEditor::setGridSize(int w, int h)
{
    _gridWidth = w;
    _gridHeight = h;
    update();
}

void WidgetEditor::setRoot(int x, int y)
{
    int const offX = _offset.x() - _root.x();
    int const offY = _offset.y() - _root.y();

    _root.setRect(x, y, 1, 1);
    _offset.setRect(_root.x()+offX, _root.y()+offY, 1, 1);

    update();
}

void WidgetEditor::setOffset(int rx, int ry)
{
    _offset.setRect(_root.x()+rx, _root.y()+ry, 1, 1);
    update();
}

void WidgetEditor::setCells(QHash<QPair<int, int>, Cell *> const * cells)
{
    _cells = cells;
    update();
}

void WidgetEditor::setBackgroundColor(QColor value)
{
    int r = (value.red() + 128) % 256;
    int g = (value.green() + 128) % 256;
    int b = (value.blue() + 128) % 256;
    int a = 128;

    _brushBackground.setColor(value);
    _penGrid.setColor(QColor::fromRgb(r,g,b,a));
    update();
}

void WidgetEditor::moveViewport(int rx, int ry)
{
    int const step = std::max(1.0, std::min(_viewport.width(), _viewport.height()) * 0.1);

    int const x = _viewport.x() + rx * step;
    int const y = _viewport.y() + ry * step;
    int const w = _viewport.width();
    int const h = _viewport.height();

    _viewport.setRect(x,y,w,h);
    update();
}

void WidgetEditor::moveViewportHome()
{
    int const x = 0;
    int const y = 0;
    int const w = _viewport.width();
    int const h = _viewport.height();

    _viewport.setRect(x,y,w,h);
    update();
}

void WidgetEditor::setZoom(int value)
{
    _viewportPower = value;
    updateViewport();
    update();
}

void WidgetEditor::updateViewport()
{
    int const centerX = _viewport.x() + _viewport.width() / 2;
    int const centerY = _viewport.y() + _viewport.height() / 2;

    int const w = std::pow(1.4, _viewportPower);
    int const h = w * height() / width();

    int const x = centerX - w / 2;
    int const y = centerY - h / 2;

    _viewport.setRect(x, y, w, h);
}

void WidgetEditor::mousePressEvent(QMouseEvent * event)
{
    int const btns = event->buttons();

    if (btns & Qt::MiddleButton)
        _lastDraggingPosition = event->pos();

    if (!(btns & Qt::LeftButton))
        return;

    auto pos = event->pos();

    int const offX = pos.x()*_viewport.width()/width();
    int const offY = pos.y()*_viewport.height()/height();

    int const x = (_viewport.x()+offX)/8-(offX<-_viewport.x()?1:0);
    int const y = (_viewport.y()+offY)/8-(offY<-_viewport.y()?1:0);

    qDebug() << pos << _viewport << x << " " << y;

    if (event->modifiers().testFlag(Qt::ShiftModifier))
        emit onLinkCell(x,y);

    else if (event->modifiers().testFlag(Qt::ControlModifier))
        emit onEraseCell(x,y);

    else if (event->modifiers().testFlag(Qt::AltModifier))
        emit onColorPickCell(x,y);

    else
        emit onPaintCell(x,y);
}

void WidgetEditor::mouseMoveEvent(QMouseEvent * event)
{
    int const btns = event->buttons();
    auto pos = event->pos();

    // Drag viewport

    if (btns & Qt::MiddleButton)
    {
        float const x = _viewport.x() - (pos.x() - _lastDraggingPosition.x()) * _viewport.width() / size().width();
        float const y = _viewport.y() - (pos.y() - _lastDraggingPosition.y()) * _viewport.height() / size().height();
        _viewport.setRect(x,y,_viewport.width(), _viewport.height());
        _lastDraggingPosition = pos;
        update();
    }

    // Emit hover events
    int const offX = pos.x()*_viewport.width()/width();
    int const offY = pos.y()*_viewport.height()/height();

    int const x = (_viewport.x()+offX)/8-(offX<-_viewport.x()?1:0);
    int const y = (_viewport.y()+offY)/8-(offY<-_viewport.y()?1:0);

    QPair<int,int> key(x,y);

    if (key == _lastHoverKey)
        return;

    _lastHoverKey = key;

    if (_cells == nullptr)
    {
        emit onHoverCell(x, y, nullptr);
    }
    else
    {
        auto it = _cells->find(key);
        emit onHoverCell(x, y, it==_cells->end() ? nullptr : it.value());
    }

}

void WidgetEditor::wheelEvent(QWheelEvent *event)
{
    _scrollPosition += event->angleDelta().ry();

    if (_scrollPosition > 0)
    {
        while (_scrollPosition >= 120)
        {
            _scrollPosition -= 120;
            emit onScrollWheel(true);
        }
    }
    else
    {
        while (_scrollPosition <= -120)
        {
            _scrollPosition += 120;
            emit onScrollWheel(false);
        }
    }
}


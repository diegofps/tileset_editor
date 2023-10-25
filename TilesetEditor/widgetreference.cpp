#include "widgetreference.h"
#include <QPainter>

WidgetReference::WidgetReference(QWidget *parent)
    : QWidget{parent},

    _img(nullptr),
    _root(128,113,8,8),
    _offset(0,0,8,8),
    _viewport(0,0,0,0),
    _viewportPower(7)
{
    _brushRoot.setColor(QColor::fromString("#6600ff00"));
    _brushRoot.setStyle(Qt::SolidPattern);

    _brushOffset.setColor(QColor::fromString("#66ff0000"));
    _brushOffset.setStyle(Qt::SolidPattern);

    updateViewport();
    update();
}

void WidgetReference::setPixmap(QPixmap *img)
{
//    qDebug() << "setPixmap|Rect:" << _root;
    if (_img != nullptr)
        delete _img;
    _img = img;
    update();
}

void WidgetReference::setRoot(int x, int y)
{
//    qDebug() << "setRoot|Rect:" << _root;
    _root.setRect(x, y, 8, 8);

    updateViewport();
    update();
}

void WidgetReference::setOffset(int rx, int ry)
{
    qDebug() << "setOffset|Offset:" << _offset;
    _offset.setRect(_root.x()+rx, _root.y()+ry, 8, 8);
    update();
}

void WidgetReference::setZoom(int value)
{
//    qDebug() << "setZoom|Rect:" << _root;
    _viewportPower = value;
    updateViewport();
    update();
}

inline void
drawRectangleInViewport(QRect const & rect,
                        QSize const & painterSize,
                        QRect const & viewport,
                        QBrush const & brush,
                        QPainter & painter)
{
#define REPROJECT(x,w,vw) (x)*(w)/(vw)

    QRect rect2(REPROJECT(rect.x()-viewport.x(), painterSize.width(), viewport.width()),
                REPROJECT(rect.y()-viewport.y(), painterSize.height(), viewport.height()),
                REPROJECT(rect.width(), painterSize.width(), viewport.width()),
                REPROJECT(rect.height(), painterSize.height(), viewport.height()));

//    qDebug() << "Rect: " << rect2.x() << "," << rect2.y() <<  " [" << rect2.width() << "" << rect2.height() << "]";
//    qDebug() << "Rect:" << rect << ", Rect2:" << rect2;
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect2);
}

void WidgetReference::paintEvent(QPaintEvent *event)
{
    (void) event;
    QPainter painter(this);

    if (_img == nullptr)
        return;

    painter.drawPixmap(QRect(0,0,width(),height()), *_img, _viewport);
    drawRectangleInViewport(_root, size(), _viewport, _brushRoot, painter);
    drawRectangleInViewport(_offset, size(), _viewport, _brushOffset, painter);
}

void WidgetReference::resizeEvent(QResizeEvent *event)
{
//    qDebug() << "resizeEvent|Rect:" << _root;
    (void) event;
    updateViewport();
    update();
}

void WidgetReference::updateViewport()
{
    int const w = std::pow(2, _viewportPower);
    int const h = w * height() / width();
    int x = _root.x() - w / 2;
    int y = _root.y() - h / 2;
    _viewport.setRect(x, y, w, h);
}


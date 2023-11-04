#include "widgetpicture.h"

#include <QPainter>
#include <QMouseEvent>

WidgetPicture::WidgetPicture(QWidget *parent)
    : QWidget{parent},

      _pixmap(nullptr),
      _rect(0,0,0,0)
{
    setMouseTracking(true);
}

void WidgetPicture::resizeEvent(QResizeEvent *event)
{
    (void) event;
    repack();
}

void WidgetPicture::paintEvent(QPaintEvent *event)
{
    (void) event;

    if (_pixmap == nullptr)
        return;

    QPainter painter(this);
    painter.drawPixmap(_rect, *_pixmap);
}

void WidgetPicture::setPixmap(QPixmap *pixmap)
{
    _pixmap = pixmap;
    repack();
    update();
}

void WidgetPicture::repack()
{
    float const w = width();
    float const h = height();

    if (_pixmap == nullptr || w == 0 || h == 0)
    {
        _rect.setRect(0,0,0,0);
        return;
    }

    float const wi = _pixmap->width();
    float const hi = _pixmap->height();

    float const a = w/h;
    float const ai = wi/hi;

    if (a > ai)
    {
        float wi2 = wi*h/hi;
        float hi2 = h;
        float x   = (w-wi2) / 2;
        float y = 0;
        _rect.setRect(x,y,wi2,hi2);
    }
    else
    {
        float hi2 = hi*w/wi;
        float wi2 = w;
        float y   = (h-hi2) / 2;
        float x = 0;
        _rect.setRect(x,y,wi2,hi2);
    }
}

void WidgetPicture::mouseMoveEvent(QMouseEvent *event)
{
    if (_pixmap == nullptr)
        return;

    auto pos = event->pos();

    int const offX = (pos.x() -_rect.x()) * _pixmap->width() / _rect.width();
    int const offY = (pos.y() - _rect.y()) * _pixmap->height() / _rect.height();

    qDebug() << pos << offX << " " << offY;

}

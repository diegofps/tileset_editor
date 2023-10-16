#include "widgetpalette.h"

#include <QPaintEvent>
#include <QPainter>

WidgetPalette::WidgetPalette(QWidget *parent)
    : QWidget{parent}
{
    _size = 0;
}

void WidgetPalette::setPalette(QColor colors[], int size)
{
    _size = size;
    for (int i=0;i!=size;++i)
        _colors[i] = colors[i];
}

void WidgetPalette::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRegion(event->region());

    for (int i=0;i!=_size;++i)
    {
        int const x0 = (i+0) * width() / _size;
        int const x1 = (i+1) * width() / _size;

        painter.setPen(Qt::NoPen);
        painter.setBrush(_colors[i]);
        painter.drawRect(QRect(x0, 0, x1-x0, height()));
    }
}

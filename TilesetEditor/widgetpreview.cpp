#include "widgetpreview.h"

#include <QPainter>

WidgetPreview::WidgetPreview(QWidget *parent)
    : QWidget{parent}
{
    setMinimumHeight(100);
}

void WidgetPreview::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(_pen);
    painter.drawLine(QPoint(0,0), QPoint(width(), height()));
    painter.drawLine(QPoint(0,height()), QPoint(width(), 0));
}

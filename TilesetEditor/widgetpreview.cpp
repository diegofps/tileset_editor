#include "widgetpreview.h"

#include <QResizeEvent>
#include <QPainter>

#define TILE_SIZE 16
#define NUM_TILES 7000

WidgetPreview::WidgetPreview(QWidget *parent)
    : QWidget{parent}
{
    setMinimumHeight(100);
    _pen.setColor(QColor::fromRgb(0,255,0));
    _brush.setColor(QColor::fromRgb(255,0,255));
    _brush.setStyle(Qt::SolidPattern);
}

void WidgetPreview::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(_pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(QPoint(0,0), QPoint(width(), height()));
    painter.drawLine(QPoint(0,height()), QPoint(width(), 0));

    painter.setPen(Qt::NoPen);
    painter.setBrush(_brush);
    painter.drawRect(0,0,TILE_SIZE,TILE_SIZE);
}

void WidgetPreview::resizeEvent(QResizeEvent *event)
{
    int cols = event->size().width() / TILE_SIZE;
    int rows = NUM_TILES / cols;

    int expectedHeight = rows * TILE_SIZE;

    if (expectedHeight != event->size().height())
        setMinimumHeight(expectedHeight);
}

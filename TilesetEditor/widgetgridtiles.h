#ifndef WIDGETGRIDTILES_H
#define WIDGETGRIDTILES_H

#include "model/tile.h"
#include <QWidget>
#include <QPen>

class ViewHolder
{
public:
    Tile * tile;
    int i;
    int j;
    QPixmap * pixmap;
    QRect rect;
//    bool selected;
};

class WidgetGridTiles : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGridTiles(QWidget *parent = nullptr);
    void setTiles(QList<Tile*> const * value);
    void setSelection(int start, int end);
    void repack();

signals:
    void onSelectedTileChanged(int start, int end);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QPen _pen;
    QBrush _brush;
    QList<Tile*> _tiles;
    int _cols;
    int _rows;
    QList<ViewHolder> _views;
    int _selectionStart;
    int _selectionEnd;
};

#endif // WIDGETGRIDTILES_H

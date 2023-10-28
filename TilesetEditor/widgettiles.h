#ifndef WIDGETTILES_H
#define WIDGETTILES_H

#include "model/tile.h"
#include "range.h"
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
};

class WidgetTiles : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTiles(QWidget *parent = nullptr);
    void setTiles(QList<Tile*> const * value);
    void setSelection(Range range);
    void repack();
    void moveTileSelection(int rx, int ry);

signals:
    void onSelectedTileChanged(Range range);

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
    Range _selection;
};

#endif // WIDGETTILES_H

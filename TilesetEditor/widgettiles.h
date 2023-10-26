#ifndef WIDGETTILES_H
#define WIDGETTILES_H

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
};

class WidgetTiles : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTiles(QWidget *parent = nullptr);
    void setTiles(QList<Tile*> const * value);
    void setSelection(int start, int end);
    void repack();
    void moveToTile(int rx, int ry);

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

#endif // WIDGETTILES_H

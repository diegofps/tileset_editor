#ifndef WIDGETGRIDTILES_H
#define WIDGETGRIDTILES_H

#include "model/tile.h"
#include <QWidget>
#include <QPen>

class WidgetGridTiles : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetGridTiles(QWidget *parent = nullptr);
    void clear();
    void append(Tile * value);
    void repack();

signals:
    void onSelectedTileChanged(Tile * value);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPen _pen;
    QBrush _brush;
    QList<Tile*> _tiles;
    int _cols;
    int _rows;

};

#endif // WIDGETGRIDTILES_H

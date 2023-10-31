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
    void repack();
    void moveTileSelection(int rx, int ry);
    void setShowLinkInfo(bool value);
    void setSelection(int position);
    void setSelection(QList<qsizetype> const * tiles);
    void clearSelection();


signals:
    void onSelectedTileChanged(QHash<qsizetype,Tile*> const & selection);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QPen _pen;
    QPen _penLinkRequired;
    QBrush _brush;
    QList<Tile*> _tiles;
    int _cols;
    int _rows;
    QList<ViewHolder> _views;
    QHash<qsizetype,Tile*> _selection;
    qsizetype _lastSelectionPosition;
    bool _showLinkInfo;
};

#endif // WIDGETTILES_H

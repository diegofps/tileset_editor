#ifndef WIDGETEDITOR_H
#define WIDGETEDITOR_H

#include "model/cell.h"
#include <QWidget>
#include <QPen>

class WidgetEditor : public QWidget
{
    Q_OBJECT

public:

    explicit WidgetEditor(QWidget *parent = nullptr);

protected:

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void updateViewport();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:

    void setGridSize(int w, int h); // Size of grid
    void setRoot(int x, int y); // Position of root cell
    void setOffset(int rx, int ry); // Position of offset cell, relative to root
    void setCells(QHash<QPair<int,int>, Cell*> const * cells);
    void setBackgroundColor(QColor value);
    void moveViewport(int rx, int ry);
    void moveViewportHome();
    void setZoom(int value);

signals:

    void onPaintCell(int x, int y); // Left click
    void onEraseCell(int x, int y); // Ctrl + Left CLick
    void onColorPickCell(int x, int y); // Alt + Left click
    void onLinkCell(int x, int y); // Shift + Left click
    void onHoverCell(Cell const * cell);

private:

    QRect _root;
    QRect _offset;
    QRect _viewport;
    int _viewportPower;
    QBrush _brushRoot;
    QBrush _brushOffset;
    QPen _penGrid;
    QBrush _brushBackground;
    int _gridWidth;
    int _gridHeight;
    QHash<QPair<int, int>, Cell *> const * _cells;

};

#endif // WIDGETEDITOR_H

#ifndef WIDGETREFERENCE_H
#define WIDGETREFERENCE_H

#include "model/reference.h"
#include <QWidget>
#include <QBrush>

class WidgetReference : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetReference(QWidget *parent = nullptr);

    QImage * offsetImage();

    void setImage(QImage * img);
    void setRoot(int x, int y);
    void setOffset(int rx, int ry);
    void setZoom(int value);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:

    void updateOffsetImage();
    void updateViewport();

private:

    QPixmap * _pixmap;
    QImage * _img;
    QImage _offsetImageBuffer;
    QImage * _offsetImage;
    QRect _root;
    QRect _offset;
    QRect _viewport;
    int _viewportPower;
    QBrush _brushRoot;
    QBrush _brushOffset;

};

#endif // WIDGETREFERENCE_H

#ifndef WIDGETPALETTE_H
#define WIDGETPALETTE_H

#include <QWidget>

class WidgetPalette : public QWidget
{
    Q_OBJECT

private:
    QColor _colors[256];
    int _size;

public:
    explicit WidgetPalette(QWidget *parent = nullptr);
    void setPalette(QColor colors[256], int size);

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // WIDGETPALETTE_H

#ifndef WIDGETCOLORPALETTE_H
#define WIDGETCOLORPALETTE_H

#include <QWidget>

class WidgetColorPalette : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetColorPalette(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // WIDGETCOLORPALETTE_H

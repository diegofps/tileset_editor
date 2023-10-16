#ifndef WIDGETITEMPALETTE_H
#define WIDGETITEMPALETTE_H

#include "model/palette.h"
#include <QWidget>

namespace Ui {
class WidgetItemPalette;
}

class WidgetItemPalette : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetItemPalette(QWidget *parent = nullptr);
    ~WidgetItemPalette();

public slots:
    void setPalette(Palette * palette);

private:
    Ui::WidgetItemPalette *ui;
};

#endif // WIDGETITEMPALETTE_H

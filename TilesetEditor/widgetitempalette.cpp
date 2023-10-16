#include "widgetitempalette.h"
#include "ui_widgetitempalette.h"

WidgetItemPalette::WidgetItemPalette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetItemPalette)
{
    ui->setupUi(this);
}

WidgetItemPalette::~WidgetItemPalette()
{
    delete ui;
}

void WidgetItemPalette::setPalette(Palette * palette)
{
    if (palette == nullptr)
    {
        ui->lbID->setText("");
        ui->palette->setPalette(nullptr, 0);
    }
    else
    {
        ui->lbID->setText(QString::number(palette->id));
        ui->palette->setPalette(palette->colors, palette->size);
    }
}

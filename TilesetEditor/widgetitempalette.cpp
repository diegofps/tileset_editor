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
    ui->lbID->setText(QString::number(palette->id));
    ui->palette->setPalette(palette->colors, palette->size);
}

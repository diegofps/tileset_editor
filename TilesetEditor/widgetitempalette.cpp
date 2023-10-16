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
        ui->lbFreq->setText("");
        ui->palette->setPalette(nullptr, 0);
    }
    else
    {
        ui->lbID->setText(QString("ID: %1").arg(palette->id));
        ui->lbFreq->setText(QString("Freq: %1").arg(palette->frequency));
        ui->palette->setPalette(palette->colors, palette->size);
    }
}

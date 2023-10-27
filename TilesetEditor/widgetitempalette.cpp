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

void WidgetItemPalette::setPalette(Palette * palette, Tile const * selectedTile)
{
    if (palette == nullptr)
    {
        ui->lbID->setText("");
        ui->palette->setPalette(nullptr, 0);
        ui->lbFreq->setText("");
    }
    else
    {
        ui->lbID->setText(QString("ID: %1").arg(palette->id));
        ui->palette->setPalette(palette->colors, palette->size);

        if (selectedTile == nullptr)
            ui->lbFreq->setText("Freq: 0");
        else
            ui->lbFreq->setText(QString("Freq: %1").arg(selectedTile->palettesUsed.value(palette->id)));
    }
}

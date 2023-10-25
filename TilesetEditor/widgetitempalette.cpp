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

void WidgetItemPalette::setPalette(Palette * palette, QList<Tile*> const * selectedTiles)
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

        if (selectedTiles == nullptr || selectedTiles->isEmpty() || !selectedTiles->at(0)->palettesUsed.contains(palette->id))
            ui->lbFreq->setText("Freq: 0");
        else
            ui->lbFreq->setText(QString("Freq: %1").arg(selectedTiles->at(0)->palettesUsed.value(palette->id)));
    }
}

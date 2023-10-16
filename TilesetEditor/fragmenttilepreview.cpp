#include "fragmenttilepreview.h"
#include "ui_fragmenttilepreview.h"

#include "app.h"

void FragmentTilePreview::styleButtons(QString const & value)
{
    if (value == "original")
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == "hd")
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btOriginal->update();
    ui->btHD->update();
}

FragmentTilePreview::FragmentTilePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilePreview)
{
    ui->setupUi(this);

    styleButtons(App::getState()->tilePreviewShow());

    connect(ui->btOriginal, &QPushButton::clicked, this, [&](){ App::getState()->setTilePreviewShow("original"); });
    connect(ui->btHD, &QPushButton::clicked, this, [&](){ App::getState()->setTilePreviewShow("hd"); });
    connect(App::getState(), &AppState::onTilePreviewShowChanged, this, [&](QString const & value) { styleButtons(value); });
    connect(App::getState(), &AppState::onPaletteSelectedItemChanged, this, [&](Palette * value) { loadPalette(value); });

    loadPalette(App::getState()->palettesSelectedItem());
}

void FragmentTilePreview::loadPalette(Palette * value)
{
    if (value == nullptr)
        qDebug() << "Changed palette to nullptr";
    else
        qDebug() << "Changed palette to " << value->id;

    ui->palettePreview->setPalette(value);
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

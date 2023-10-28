#include "fragmenttilepreview.h"
#include "ui_fragmenttilepreview.h"

#include "app.h"

void FragmentTilePreview::styleButton(QPushButton * btn, bool value)
{
    btn->setStyleSheet(App::getStyles()->get(value?"button_checked":"button_unchecked"));
    btn->update();
}

FragmentTilePreview::FragmentTilePreview(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilePreview)
{
    ui->setupUi(this);

    TileFilter & tileMode = App::getState()->tileMode();
    styleButton(ui->btHFlip, tileMode.hFlip);
    styleButton(ui->btVFlip, tileMode.vFlip);

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        TileFilter filter = App::getState()->tileMode();
        filter.hFlip = !filter.hFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        TileFilter tileMode = App::getState()->tileMode();
        tileMode.vFlip = !tileMode.vFlip;
        App::getState()->setTilePreviewFilter(tileMode);
    });

    connect(App::getState(), &AppState::onTileFilterChanged, this, [&](TileFilter const & filter)
    {
        styleButton(ui->btHFlip, filter.hFlip);
        styleButton(ui->btVFlip, filter.vFlip);
        updateTileWidget();
    });

    connect(App::getState(), &AppState::onSelectedPaletteChanged, this, [&](Palette *)
    {
        updateTileWidget();
        updatePaletteWidget();
    });

    connect(App::getState(), &AppState::onSelectedTilesChanged, this, [&](QList<Tile*> const *)
    {
        updateTileWidget();
    });

    updateTileWidget();
    updatePaletteWidget();
}

void FragmentTilePreview::updateTileWidget()
{
    TileFilter const & mode = App::getState()->tileMode();
    Tile * selectedTile = App::getState()->selectedTile();
    Palette * palette = App::getState()->selectedPalette();

    ui->picture->setPixmap(nullptr);

    if (selectedTile == nullptr || palette == nullptr)
        return;

//    QPixmap * img = mode == "original"
//            ? App::getOriginalTileCache()->getTilePixmap(tile, palette, mode.hFlip, mode.vFlip)
//            : App::getHDTileCache()->getTilePixmap(tile, palette);

    QPixmap * img = App::getOriginalTileCache()->getTilePixmap(selectedTile, palette, mode.hFlip, mode.vFlip);

    ui->picture->setPixmap(img);
}

void FragmentTilePreview::updatePaletteWidget()
{
    auto palette = App::getState()->selectedPalette();
    auto selectedTile = App::getState()->selectedTile();
    ui->palettePreview->setPalette(palette, selectedTile);
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

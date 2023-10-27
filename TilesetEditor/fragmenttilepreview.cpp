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

    TileMode & tileMode = App::getState()->tileMode();
    styleButton(ui->btHFlip, tileMode.hFlip);
    styleButton(ui->btVFlip, tileMode.vFlip);

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        TileMode tileMode = App::getState()->tileMode();
        tileMode.hFlip = !tileMode.hFlip;
        App::getState()->setTileMode(tileMode);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        TileMode tileMode = App::getState()->tileMode();
        tileMode.vFlip = !tileMode.vFlip;
        App::getState()->setTileMode(tileMode);
    });

    connect(App::getState(), &AppState::onTileModeChanged, this, [&](TileMode const & value)
    {
        styleButton(ui->btHFlip, value.hFlip);
        styleButton(ui->btVFlip, value.vFlip);

        TileMode const & tileMode = App::getState()->tileMode();
        Tile * tiles = App::getState()->selectedTile();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
    });

    connect(App::getState(), &AppState::onSelectedPaletteChanged, this, [&](Palette * value) {

        TileMode const & tileMode = App::getState()->tileMode();
        Tile * tiles = App::getState()->selectedTile();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
        updatePaletteWidget(value, tiles);
    });

    connect(App::getState(), &AppState::onSelectedTilesPosChanged, this, [&](Range) {

        TileMode const & tileMode = App::getState()->tileMode();
        Tile * tiles = App::getState()->selectedTile();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
    });

    Tile * tiles = App::getState()->selectedTile();
    Palette * palette = App::getState()->selectedPalette();

    updateTileWidget(tileMode, tiles, palette);
    updatePaletteWidget(App::getState()->selectedPalette(), tiles);
}

void FragmentTilePreview::updateTileWidget(TileMode const & mode, Tile * selectedTile, Palette * palette)
{
    ui->picture->setPixmap(nullptr);

    if (selectedTile == nullptr || palette == nullptr)
        return;

//    QPixmap * img = mode == "original"
//            ? App::getOriginalTileCache()->getTilePixmap(tile, palette, mode.hFlip, mode.vFlip)
//            : App::getHDTileCache()->getTilePixmap(tile, palette);

    QPixmap * img = App::getOriginalTileCache()->getTilePixmap(selectedTile, palette, mode.hFlip, mode.vFlip);

    ui->picture->setPixmap(img);
}

void FragmentTilePreview::updatePaletteWidget(Palette * value, Tile * selectedTile)
{
    ui->palettePreview->setPalette(value, selectedTile);
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

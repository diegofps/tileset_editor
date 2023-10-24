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
        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
    });

    connect(App::getState(), &AppState::onSelectedPaletteChanged, this, [&](Palette * value) {
        updatePaletteWidget(value);

        TileMode const & tileMode = App::getState()->tileMode();
        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
    });

    connect(App::getState(), &AppState::onTilesSelectedItemsChanged, this, [&](QList<Tile*> *) {

        TileMode const & tileMode = App::getState()->tileMode();
        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
        Palette * palette = App::getState()->selectedPalette();

        updateTileWidget(tileMode, tiles, palette);
    });

    updatePaletteWidget(App::getState()->selectedPalette());
}

void FragmentTilePreview::updateTileWidget(TileMode const & mode, QList<Tile*> const * tiles, Palette * palette)
{
    if (tiles == nullptr || tiles->isEmpty() || palette == nullptr)
    {
        ui->picture->setPixmap(nullptr);
        return;
    }

    Tile * tile = (*tiles)[0];

//    QPixmap * img = mode == "original"
//            ? App::getOriginalTileCache()->getTilePixmap(tile, palette, mode.hFlip, mode.vFlip)
//            : App::getHDTileCache()->getTilePixmap(tile, palette);

    QPixmap * img = App::getOriginalTileCache()->getTilePixmap(tile, palette, mode.hFlip, mode.vFlip);

    ui->picture->setPixmap(img);
}

void FragmentTilePreview::updatePaletteWidget(Palette * value)
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

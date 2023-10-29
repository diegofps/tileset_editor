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

    TilePreviewFilter & tileMode = App::getState()->tilePreviewFilter();
    styleButton(ui->btHFlip, tileMode.hFlip);
    styleButton(ui->btVFlip, tileMode.vFlip);

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        TilePreviewFilter filter = App::getState()->tilePreviewFilter();
        filter.hFlip = !filter.hFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        TilePreviewFilter tileMode = App::getState()->tilePreviewFilter();
        tileMode.vFlip = !tileMode.vFlip;
        App::getState()->setTilePreviewFilter(tileMode);
    });

    connect(App::getState(), &AppState::onTileFilterChanged, this, [&](TilePreviewFilter const & filter)
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
    TilePreviewFilter const & mode = App::getState()->tilePreviewFilter();
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

    if (palette == nullptr || selectedTile == nullptr)
        ui->lbExtraInfo->setText("");
    else
        ui->lbExtraInfo->setText(QString("TID:%1, PID:%2, LinkID:%3, FavPID:%4, S:%5, B:%6, H:%7, V:%8")
                                 .arg(selectedTile->id)
                                 .arg(palette->id)
                                 .arg(selectedTile->linkedCellID)
                                 .arg(selectedTile->favoritePaletteID)
                                 .arg(selectedTile->usedInSprite)
                                 .arg(selectedTile->usedInBackground)
                                 .arg(selectedTile->usedWithHFlip())
                                 .arg(selectedTile->usedWithVFlip()));

}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

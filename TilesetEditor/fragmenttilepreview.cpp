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

    styleButtons(App::getState()->tilePreviewMode());

    connect(ui->btOriginal, &QPushButton::clicked, this, [&](){ App::getState()->setTilePreviewMode("original"); });
    connect(ui->btHD, &QPushButton::clicked, this, [&](){ App::getState()->setTilePreviewMode("hd"); });

    connect(App::getState(), &AppState::onTilePreviewModeChanged, this, [&](QString const & value) {
        styleButtons(value);

        QString const & mode = App::getState()->tilePreviewMode();
        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
        Palette * palette = App::getState()->selectedPalette();

        loadTile(mode, tiles, palette);
    });

    connect(App::getState(), &AppState::onSelectedPaletteChanged, this, [&](Palette * value) {
        loadPalette(value);

        QString const & mode = App::getState()->tilePreviewMode();
        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
//        Palette * palette = App::getState()->palettesSelectedItem();

        loadTile(mode, tiles, value);
    });

    connect(App::getState(), &AppState::onTilesSelectedItemsChanged, this, [&](QList<Tile*> * value) {

        QString const & mode = App::getState()->tilePreviewMode();
//        QList<Tile*> const * tiles = App::getState()->tilesElectedItems();
        Palette * palette = App::getState()->selectedPalette();

        loadTile(mode, value, palette);
    });

    loadPalette(App::getState()->selectedPalette());
}

void FragmentTilePreview::loadTile(QString mode, QList<Tile*> const * tiles, Palette * palette)
{
    if (tiles == nullptr || tiles->isEmpty() || palette == nullptr || (mode!="hd" && mode!="original"))
    {
        ui->picture->setPixmap(nullptr);
        return;
    }

    Tile * tile = (*tiles)[0];

    QPixmap * img = mode == "original"
            ? App::getOriginalTileCache()->getTilePixmap(tile, palette)
            : App::getHDTileCache()->getTilePixmap(tile, palette);

    ui->picture->setPixmap(img);
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

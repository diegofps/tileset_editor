#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

#include "app.h"
#include "widgetitempalette.h"

void FragmentPalettes::styleButtons(PaletteMode value)
{
    if (value == ALL)
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == USED_BY_TILE)
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btAll->update();
    ui->btUsedWithTile->update();
}

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);

    styleButtons(App::getState()->palettesMode());

    connect(ui->btAll, &QPushButton::clicked, this, [&]()
    {
        App::getState()->setPalettesMode(ALL);
    });

    connect(ui->btUsedWithTile, &QPushButton::clicked, this, [&]()
    {
        App::getState()->setPalettesMode(USED_BY_TILE);
    });

    connect(App::getState(), &AppState::onPalettesModeChanged, this, [&](PaletteMode value)
    {
        styleButtons(value);

        auto selectedTiles = App::getState()->selectedTiles();
        auto selectedTile = App::getState()->selectedTile();
        auto palettes = App::getState()->projectPalettes();
        auto paletteMode = App::getState()->palettesMode();

        saveSelectedPalette();
        filterPalettes(selectedTiles, palettes, paletteMode);
        updatePalettesWidget(selectedTile);
        restoreSelectedPalette(selectedTile);
    });

    connect(App::getState(), &AppState::onSelectedTilesPosChanged, this, [&](Range)
    {
        auto selectedTiles = App::getState()->selectedTiles();
        auto selectedTile = App::getState()->selectedTile();
        auto palettes = App::getState()->projectPalettes();
        auto paletteMode = App::getState()->palettesMode();

        saveSelectedPalette();
        filterPalettes(selectedTiles, palettes, paletteMode);
        updatePalettesWidget(selectedTile);
        restoreSelectedPalette(selectedTile);

    });

    connect(App::getState(), &AppState::onProjectPalettesChanged, this, [&](QList<Palette*> const *)
    {
        auto selectedTiles = App::getState()->selectedTiles();
        auto selectedTile = App::getState()->selectedTile();
        auto palettes = App::getState()->projectPalettes();
        auto paletteMode = App::getState()->palettesMode();

        saveSelectedPalette();
        filterPalettes(selectedTiles, palettes, paletteMode);
        updatePalettesWidget(selectedTile);
        restoreSelectedPalette(selectedTile);
    });

    connect(ui->listPalettes, &QListWidget::currentRowChanged, this, [&](int position)
    {
        if (position < 0 || position >= _palettes.size())
            App::getState()->setSelectedPalette(nullptr);
        else
            App::getState()->setSelectedPalette(_palettes[position]);
    });


    auto selectedTiles = App::getState()->selectedTiles();
    auto selectedTile = App::getState()->selectedTile();
    auto palettes = App::getState()->projectPalettes();
    auto paletteMode = App::getState()->palettesMode();

//    saveSelectedPalette();
    filterPalettes(selectedTiles, palettes, paletteMode);
    updatePalettesWidget(selectedTile);
//    restoreSelectedPalette();

    if (!_palettes.isEmpty())
    {
        int bestI = 0;
        int bestF = _palettes[0]->frequency;

        for (qsizetype i=1;i!=_palettes.size();++i)
        {
            if (_palettes[i]->frequency > bestF)
            {
                bestF = _palettes[i]->frequency;
                bestI = i;
            }
        }
        ui->listPalettes->setCurrentRow(bestI);
    }
//    ui->listPalettes->setCurrentRow(0);
}

void FragmentPalettes::filterPalettes(QList<Tile*> const & selectedTiles, QList<Palette*> const * value, PaletteMode paletteMode)
{
    _palettes.clear();

    if (value == nullptr)
        return;

    if (paletteMode == ALL)
    {
        for (auto item : *value)
            _palettes.append(item);
        qDebug() << "Filter found " << _palettes.size() << " palettes using all";
        return;
    }

    if (selectedTiles.isEmpty())
        return;

    for (auto palette : *value)
    {
        bool add = false;

        for (auto tile : selectedTiles)
        {
            if (tile != nullptr && tile->palettesUsed.contains(palette->id))
            {
                add = true;
                break;
            }
        }

        if (add)
            _palettes.append(palette);
    }

    qDebug() << "Filter found " << _palettes.size() << " palettes not using all";
}

void FragmentPalettes::updatePalettesWidget(Tile * selectedTile)
{
    ui->listPalettes->clear();

    if (selectedTile == nullptr)
        return;

    for (auto p : _palettes)
    {
        auto itemWidget = new WidgetItemPalette(this);
        itemWidget->setPalette(p, selectedTile);

        auto item = new QListWidgetItem();
        item->setSizeHint(itemWidget->sizeHint());

        ui->listPalettes->addItem(item);
        ui->listPalettes->setItemWidget(item, itemWidget);
    }

    ui->listPalettes->update();
    qDebug() << "updatePaletteWidget added " << ui->listPalettes->size() << " palettes";
}

void FragmentPalettes::saveSelectedPalette()
{
    auto palette = App::getState()->selectedPalette();

    if (palette == nullptr)
        _lastSelectedItemID = 0;
    else
        _lastSelectedItemID = palette->id;
    qDebug() << "Saving " << _lastSelectedItemID;
}

void FragmentPalettes::restoreSelectedPalette(Tile * selectedTile)
{
    if (_palettes.isEmpty())
        return;

    if (_lastSelectedItemID != 0)
    {
        for (qsizetype i=0;i!=_palettes.size();++i)
        {
            if (_palettes[i]->id == _lastSelectedItemID)
            {
                qDebug() << "Found id " << _lastSelectedItemID << " at position " << i;
                ui->listPalettes->setCurrentRow(i);
                return;
            }
        }
    }

    if (selectedTile != nullptr)
    {
        int const preferredTile = selectedTile->preferredPalette;

        for (qsizetype i=0;i!=_palettes.size();++i)
        {
            if (_palettes[i]->id == preferredTile)
            {
                qDebug() << "Found preferred tile id " << preferredTile << " at position " << i;
                ui->listPalettes->setCurrentRow(i);
                return;
            }
        }
    }

    qDebug() << "Could not find last selected palette with id " << _lastSelectedItemID << " or a preferred tile, showing palette at position 0";
    ui->listPalettes->setCurrentRow(0);
}

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

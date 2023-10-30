#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

#include "app.h"
#include "widgetitempalette.h"

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);

    updateFilterButtons(App::getState()->palettesMode());

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
        updateFilterButtons(value);
    });

    connect(App::getState(), &AppState::onFilteredPalettesChanged, this, [&](QList<Palette*> const *)
    {
        saveSelectedPalette();
        updatePalettesWidget();
        restoreSelectedPalette();
    });

    connect(ui->listPalettes, &QListWidget::currentRowChanged, this, [&](int position)
    {
        auto palettes = App::getState()->filteredPalettes();
        auto palette = position < 0 || position >= palettes->size()?nullptr:palettes->at(position);
        App::getState()->setSelectedPalette(palette);
    });

    updatePalettesWidget();
    auto palettes = App::getState()->filteredPalettes();

    if (!palettes->isEmpty())
    {
        int bestI = 0;
        int bestF = palettes->at(0)->frequency;

        for (qsizetype i=1;i!=palettes->size();++i)
        {
            if (palettes->at(i)->frequency > bestF)
            {
                bestF = palettes->at(i)->frequency;
                bestI = i;
            }
        }
        ui->listPalettes->setCurrentRow(bestI);
    }
//    ui->listPalettes->setCurrentRow(0);
}

void FragmentPalettes::updatePalettesWidget()
{
    auto selectedTile = App::getState()->selectedTile();
    ui->listPalettes->clear();

    for (auto p : *App::getState()->filteredPalettes())
    {
        auto itemWidget = new WidgetItemPalette(this);
        itemWidget->setPalette(p, selectedTile);

        auto item = new QListWidgetItem();
        item->setSizeHint(itemWidget->sizeHint());

        ui->listPalettes->addItem(item);
        ui->listPalettes->setItemWidget(item, itemWidget);
    }

    ui->listPalettes->update();
//    qDebug() << "updatePaletteWidget added " << ui->listPalettes->size() << " palettes";
}

void FragmentPalettes::saveSelectedPalette()
{
    auto palette = App::getState()->selectedPalette();
    _lastSelectedItemID = palette == nullptr?0:palette->id;
//    qDebug() << "Saving " << _lastSelectedItemID;
}

void FragmentPalettes::restoreSelectedPalette()
{
    auto palettes = App::getState()->filteredPalettes();

    if (palettes->isEmpty())
        return;

    if (_lastSelectedItemID != 0)
    {
        for (qsizetype i=0;i!=palettes->size();++i)
        {
            if (palettes->at(i)->id == _lastSelectedItemID)
            {
//                qDebug() << "Found id " << _lastSelectedItemID << " at position " << i;
                ui->listPalettes->setCurrentRow(i);
                return;
            }
        }
    }

    auto selectedTile = App::getState()->selectedTile();

    if (selectedTile != nullptr)
    {
        int const preferredTile = selectedTile->favoritePaletteID;

        for (qsizetype i=0;i!=palettes->size();++i)
        {
            if (palettes->at(i)->id == preferredTile)
            {
//                qDebug() << "Found preferred tile id " << preferredTile << " at position " << i;
                ui->listPalettes->setCurrentRow(i);
                return;
            }
        }
    }

    if (ui->listPalettes->count() != 0)
        ui->listPalettes->setCurrentRow(0);

    qDebug() << "Could not find last selected palette with id " << _lastSelectedItemID << " or a preferred tile, showing palette at position 0";
}

void FragmentPalettes::updateFilterButtons(PaletteMode value)
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

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

#include "fragmenttiles.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgettiles.h"
#include "widgetvscrollarea.h"

inline void nextButtonState(int & s)
{
    if (s == 0) s = 1;
    else if (s == 1) s = 2;
    else s = 0;
}

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);

    _widgetTiles = new WidgetTiles();

    auto vScrollArea = new WidgetVScrollArea(_widgetTiles, this);
    vScrollArea->setContentsMargins(0,0,0,0);

    auto layout2 = new QVBoxLayout();
    layout2->setContentsMargins(0,0,0,0);
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);

    connect(ui->btSprite, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedInSprite);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedInBackground);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedWithHFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedWithVFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.isUnlinked);
        App::getState()->setTilesFilter(filter);
    });

    connect(App::getState(), &AppState::onTilesFilterChanged, this, [&](TilesFilter const &)
    {
        updateFilterWidgets();
    });

    connect(App::getState(), &AppState::onFilteredTilesChanged, this, [&](QList<Tile*> const *)
    {
        saveSelectedTile();
        updateTilesWidget();
        restoreSelectedTile();
    });

    connect(App::getState(), &AppState::onShowLinkInfoChanged, this, [&](bool value)
    {
        _widgetTiles->setShowLinkInfo(value);
    });

//    connect(App::getState(), &AppState::onSelectedTilesChanged, this, [&](QList<Tile*> tiles)
//    {
//        _widgetTiles->setSelection(tilesPos.isEmpty()?Range(0,0):tilesPos);
//    });

    connect(_widgetTiles, &WidgetTiles::onSelectedTileChanged, this, [&](Range range)
    {
        if (range.start>=0 && range.end>=0)
        {
            QList<Tile*> selectedTiles;
            auto tiles = App::getState()->filteredTiles();

            for (int i=range.start;i<=range.end;++i)
                selectedTiles.append(tiles->at(i));

            App::getState()->setSelectedTiles(selectedTiles);
        }
    });

    connect(App::getState(), &AppState::onMoveTileSelection, this, [&](int rx, int ry)
    {
        _widgetTiles->moveTileSelection(rx, ry);
    });

    updateFilterWidgets();
    updateTilesWidget();
    _widgetTiles->setSelection(Range(0,0));

}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

void FragmentTiles::updateFilterWidgets()
{
    auto filter = App::getState()->tilesFilter();
    styleButton(ui->btBackground, filter.usedInBackground);
    styleButton(ui->btUnlinked, filter.isUnlinked);
    styleButton(ui->btSprite, filter.usedInSprite);
    styleButton(ui->btHFlip, filter.usedWithHFlip);
    styleButton(ui->btVFlip, filter.usedWithVFlip);
}

void FragmentTiles::updateTilesWidget()
{
    _widgetTiles->setShowLinkInfo(App::getState()->showLinkInfo());
    _widgetTiles->setTiles(App::getState()->filteredTiles());
    _widgetTiles->repack();
    _widgetTiles->update();
}

void FragmentTiles::saveSelectedTile()
{
    auto tile = App::getState()->selectedTile();
    _lastSelectedItemID = tile == nullptr ? 0 : tile->id;
};

void FragmentTiles::restoreSelectedTile()
{
    auto tiles = App::getState()->filteredTiles();

    if (tiles->isEmpty())
        return;

    if (_lastSelectedItemID != 0)
    {
        for (qsizetype i=0;i!=tiles->size();++i)
        {
            if (tiles->at(i)->id == _lastSelectedItemID)
            {
                _widgetTiles->setSelection(Range(i,i));
                return;
            }
        }
    }

    _widgetTiles->setSelection(Range(0,0));
};

void FragmentTiles::styleButton(QPushButton * btn, int const value)
{
    auto stylesheet = App::getStyles()->get(value==0
                                              ?"button_unchecked"
                                              : value==1
                                                ?"button_checked"
                                                : "button_any");

    if (btn->styleSheet() != stylesheet)
    {
        btn->setStyleSheet(stylesheet);
        btn->update();
    }
}

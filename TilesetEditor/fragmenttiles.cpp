#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgetgridtiles.h"
#include "widgetvscrollarea.h"

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

    auto filter = App::getState()->tilesFilter();

    styleButton(ui->btBackground, filter->usedInBackground);
    styleButton(ui->btUnlinked, filter->isUnlinked);
    styleButton(ui->btSprite, filter->usedInSprite);
    styleButton(ui->btHFlip, filter->usedWithHFlip);
    styleButton(ui->btVFlip, filter->usedWithVFlip);

    connect(ui->btSprite, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextButtonState(filter->usedInSprite);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextButtonState(filter->usedInBackground);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextButtonState(filter->usedWithHFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextButtonState(filter->usedWithVFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextButtonState(filter->isUnlinked);
        App::getState()->setTilesFilter(filter);
    });

    connect(App::getState(), &AppState::onProjectSelectedSceneIDChanged, this, [&](int value) {
        auto tiles = App::getState()->projectTiles();
        auto filter = App::getState()->tilesFilter();

        filterTiles(value, tiles, filter);
        updateTilesWidget();
        _gridTiles->setSelection(0,0);
    });

    connect(App::getState(), &AppState::onTilesFilterChanged, this, [&](TilesFilter * filter)
    {
        styleButton(ui->btBackground, filter->usedInBackground);
        styleButton(ui->btSprite, filter->usedInSprite);
        styleButton(ui->btUnlinked, filter->isUnlinked);
        styleButton(ui->btHFlip, filter->usedWithHFlip);
        styleButton(ui->btVFlip, filter->usedWithVFlip);

        saveSelectedTile();
        filterTiles(App::getState()->projectSelectedSceneID(), App::getState()->projectTiles(), filter);
        updateTilesWidget();
        restoreSelectedTile();
    });

    connect(App::getState(), &AppState::onProjectTilesChanged, this, [&](QList<Tile*> const * value)
    {
        saveSelectedTile();
        filterTiles(App::getState()->projectSelectedSceneID(), value, App::getState()->tilesFilter());
        updateTilesWidget();
        restoreSelectedTile();
    });

    _gridTiles = new WidgetGridTiles(this);

    auto vScrollArea = new WidgetVScrollArea(_gridTiles, this);
    vScrollArea->setContentsMargins(0,0,0,0);

    auto layout2 = new QVBoxLayout();
    layout2->setContentsMargins(0,0,0,0);
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);

    filterTiles(App::getState()->projectSelectedSceneID(), App::getState()->projectTiles(), App::getState()->tilesFilter());
    updateTilesWidget();

    connect(_gridTiles, &WidgetGridTiles::onSelectedTileChanged, this, [&](int start, int end) {
        if (start<0 || end <0)
            return;

        QList<Tile*> selectedTiles;

        for (int i=start;i<=end;++i)
            selectedTiles.append(_tiles[i]);

        App::getState()->setTilesSelectedItems(&selectedTiles);
    });

    _gridTiles->setSelection(0,0);

}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

void FragmentTiles::filterTiles(int sceneID, QList<Tile*> const * tiles, TilesFilter * filter)
{
    _tiles.clear();

    if (tiles == nullptr)
        return;

    if (filter == nullptr)
    {
        if (sceneID==-1)
        {
            for (auto t : *tiles)
                _tiles.append(t);
        }
        else
        {
            for (auto t : *tiles)
                if (t->sceneId == sceneID)
                    _tiles.append(t);
        }
    }

    else
    {
        QHash<int, int> unlinkedTiles;

        if (filter->isUnlinked)
        {
            // TODO
        }

        for (auto t : *tiles)
        {
            if (sceneID!=-1)
                if (t->sceneId!=sceneID)
                    continue;

            if (filter->isUnlinked!=2)
                if ((filter->isUnlinked==0 && unlinkedTiles.contains(t->id)) || (filter->isUnlinked==1 && !unlinkedTiles.contains(t->id)))
                    continue;

            if (filter->usedInSprite!=2)
                if ((filter->usedInSprite==0 && t->usedInSprite) || (filter->usedInSprite==1 && !t->usedInSprite))
                    continue;

            if (filter->usedInBackground!=2)
                if ((filter->usedInBackground==0 && t->usedInBackground) || (filter->usedInBackground==1 && !t->usedInBackground))
                    continue;

            if (filter->usedWithHFlip!=2)
                if ((filter->usedWithHFlip==0 && t->usedWithHFlip) || (filter->usedWithHFlip==1 && !t->usedWithHFlip))
                    continue;

            if (filter->usedWithVFlip!=2)
                if ((filter->usedWithVFlip==0 && t->usedWithVFlip) || (filter->usedWithVFlip==1 && !t->usedWithVFlip))
                    continue;

            _tiles.append(t);
        }
    }
}

void FragmentTiles::updateTilesWidget()
{
    _gridTiles->setTiles(&_tiles);
    _gridTiles->repack();
    _gridTiles->update();
}

void FragmentTiles::saveSelectedTile()
{
    auto selectedItems = App::getState()->tilesElectedItems();
    if (selectedItems == nullptr || selectedItems->isEmpty())
    {
        _lastSelectedItemID = 0;
        return;
    }

    auto item = (*selectedItems)[0];
    _lastSelectedItemID = item->id;
};

void FragmentTiles::restoreSelectedTile()
{
    if (_lastSelectedItemID != 0)
    for (qsizetype i=0;i!=_tiles.size();++i)
        if (_tiles[i]->id == _lastSelectedItemID)
        {
            _gridTiles->setSelection(i,i);
            return;
        }
    _gridTiles->setSelection(0,0);
};

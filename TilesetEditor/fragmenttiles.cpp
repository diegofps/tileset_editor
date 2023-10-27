#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgettiles.h"
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

    styleButton(ui->btBackground, filter.usedInBackground);
    styleButton(ui->btUnlinked, filter.isUnlinked);
    styleButton(ui->btSprite, filter.usedInSprite);
    styleButton(ui->btHFlip, filter.usedWithHFlip);
    styleButton(ui->btVFlip, filter.usedWithVFlip);

    connect(ui->btSprite, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedInSprite);
        App::getCommands()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedInBackground);
        App::getCommands()->setTilesFilter(filter);
    });

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedWithHFlip);
        App::getCommands()->setTilesFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.usedWithVFlip);
        App::getCommands()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        TilesFilter filter = App::getState()->tilesFilter();
        nextButtonState(filter.isUnlinked);
        App::getCommands()->setTilesFilter(filter);
    });

    connect(App::getState(), &AppState::onSelectedScenePosChanged, this, [&](int scenePos)
    {
        auto tiles = App::getState()->projectTiles();
        auto filter = App::getState()->tilesFilter();
        auto sceneID = App::getState()->scenePos2ID(scenePos);

        filterTiles(sceneID, tiles, filter);
        updateTilesWidget();
        _widgetTiles->setSelection(Range(0,0));
    });

    connect(App::getState(), &AppState::onTilesFilterChanged, this, [&](TilesFilter const & filter)
    {
        styleButton(ui->btBackground, filter.usedInBackground);
        styleButton(ui->btSprite, filter.usedInSprite);
        styleButton(ui->btUnlinked, filter.isUnlinked);
        styleButton(ui->btHFlip, filter.usedWithHFlip);
        styleButton(ui->btVFlip, filter.usedWithVFlip);

        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());

        saveSelectedTile();
        filterTiles(sceneID, App::getState()->projectTiles(), filter);
        updateTilesWidget();
        restoreSelectedTile();
    });

    connect(App::getState(), &AppState::onProjectTilesChanged, this, [&](QList<Tile*> const * value)
    {
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());

        saveSelectedTile();
        filterTiles(sceneID, value, App::getState()->tilesFilter());
        updateTilesWidget();
        restoreSelectedTile();
    });

    connect(App::getState(), &AppState::onSelectedTilesPosChanged, this, [&](Range tilesPos)
    {
        if (tilesPos.isEmpty())
            _widgetTiles->setSelection(Range(0,0));
        else
            _widgetTiles->setSelection(tilesPos);
    });

    _widgetTiles = new WidgetTiles();

    auto vScrollArea = new WidgetVScrollArea(_widgetTiles, this);
    vScrollArea->setContentsMargins(0,0,0,0);

    auto layout2 = new QVBoxLayout();
    layout2->setContentsMargins(0,0,0,0);
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);

    auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());
    filterTiles(sceneID, App::getState()->projectTiles(), App::getState()->tilesFilter());
    updateTilesWidget();

    connect(_widgetTiles, &WidgetTiles::onSelectedTileChanged, this, [&](Range range)
    {
        if (range.start<0 || range.end<0)
            return;

        App::getCommands()->setSelectedTilesPos(range);
    });

    connect(App::getState(), &AppState::onSelectTileAtEditorPosition, this, [&](int rx, int ry)
    {
        _widgetTiles->moveToTile(rx, ry);
    });

    _widgetTiles->setSelection(Range(0,0));

}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

void FragmentTiles::filterTiles(int sceneID, QList<Tile*> const * tiles, TilesFilter const & filter)
{
    _tiles.clear();

    if (tiles == nullptr)
        return;


    QHash<int, int> unlinkedTiles;

    if (filter.isUnlinked)
    {
        // TODO
    }

    for (auto t : *tiles)
    {
        if (sceneID!=-1)
            if (t->sceneId!=sceneID)
                continue;

        if (filter.isUnlinked!=2)
            if ((filter.isUnlinked==0 && unlinkedTiles.contains(t->id)) || (filter.isUnlinked==1 && !unlinkedTiles.contains(t->id)))
                continue;

        if (filter.usedInSprite!=2)
            if ((filter.usedInSprite==0 && t->usedInSprite) || (filter.usedInSprite==1 && !t->usedInSprite))
                continue;

        if (filter.usedInBackground!=2)
            if ((filter.usedInBackground==0 && t->usedInBackground) || (filter.usedInBackground==1 && !t->usedInBackground))
                continue;

        if (filter.usedWithHFlip!=2)
            if ((filter.usedWithHFlip==0 && t->usedWithHFlip()) || (filter.usedWithHFlip==1 && !t->usedWithHFlip()))
                continue;

        if (filter.usedWithVFlip!=2)
            if ((filter.usedWithVFlip==0 && t->usedWithVFlip()) || (filter.usedWithVFlip==1 && !t->usedWithVFlip()))
                continue;

        _tiles.append(t);
    }
}

void FragmentTiles::updateTilesWidget()
{
    _widgetTiles->setTiles(&_tiles);
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
    if (_tiles.isEmpty())
        return;

    if (_lastSelectedItemID != 0)
        for (qsizetype i=0;i!=_tiles.size();++i)
            if (_tiles[i]->id == _lastSelectedItemID)
            {
                _widgetTiles->setSelection(Range(i,i));
                return;
            }
    _widgetTiles->setSelection(Range(0,0));
};

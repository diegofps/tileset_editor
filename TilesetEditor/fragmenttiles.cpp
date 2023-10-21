#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgetgridtiles.h"
#include "qverticalscrollarea.h"

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

inline void nextState(int & s)
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
        nextState(filter->usedInSprite);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextState(filter->usedInBackground);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btHFlip, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextState(filter->usedWithHFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btVFlip, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextState(filter->usedWithVFlip);
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        nextState(filter->isUnlinked);
        App::getState()->setTilesFilter(filter);
    });

    connect(App::getState(), &AppState::onTilesFilterChanged, this, [&](TilesFilter * filter)
    {
        loadTiles(App::getState()->projectTiles(), filter);
    });

    connect(App::getState(), &AppState::onProjectTilesChanged, this, [&](QList<Tile*> const * value)
    {
        loadTiles(value, App::getState()->tilesFilter());
    });

    _gridTiles = new WidgetGridTiles(this);

    auto vScrollArea = new QVerticalScrollArea(_gridTiles, this);
    vScrollArea->setContentsMargins(0,0,0,0);

    auto layout2 = new QVBoxLayout();
    layout2->setContentsMargins(0,0,0,0);
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);

    loadTiles(App::getState()->projectTiles(), App::getState()->tilesFilter());
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

void FragmentTiles::loadTiles(QList<Tile*> const * tiles, TilesFilter * filter)
{
    styleButton(ui->btBackground, filter->usedInBackground);
    styleButton(ui->btSprite, filter->usedInSprite);
    styleButton(ui->btUnlinked, filter->isUnlinked);
    styleButton(ui->btHFlip, filter->usedWithHFlip);
    styleButton(ui->btVFlip, filter->usedWithVFlip);

    _gridTiles->clear();

    if (tiles != nullptr)
    {
        if (filter == nullptr)
        {
            for (auto t : *tiles)
                _gridTiles->append(t);
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
                if (filter->clusterID!=-1)
                    if (t->clusterId!=filter->clusterID)
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

                _gridTiles->append(t);
            }
        }
    }

    _gridTiles->repack();
    _gridTiles->update();
}

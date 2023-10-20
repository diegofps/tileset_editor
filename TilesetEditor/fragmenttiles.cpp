#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgetgridtiles.h"
#include "qverticalscrollarea.h"

void FragmentTiles::styleButton(QPushButton * btn, bool const value)
{
    auto stylesheet = App::getStyles()->get(value?"button_checked":"button_unchecked");

    if (btn->styleSheet() != stylesheet)
    {
        btn->setStyleSheet(stylesheet);
        btn->update();
    }
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

    connect(ui->btSprite, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->usedInSprite = !filter->usedInSprite;
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->usedInBackground = !filter->usedInBackground;
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->isUnlinked = !filter->isUnlinked;
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
//    vScrollArea->setSpacing(6);

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
                if ((!filter->usedInSprite || t->usedInSprite) &&
                    (!filter->usedInBackground || t->usedInBackground) &&
                    (!filter->isUnlinked || unlinkedTiles.contains(t->id)))
                {
                    _gridTiles->append(t);
                }
            }
        }
    }

    _gridTiles->repack();
    _gridTiles->update();
}

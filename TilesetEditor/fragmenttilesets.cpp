#include "fragmenttilesets.h"
#include "ui_fragmenttilesets.h"

#include "app.h"

FragmentTilesets::FragmentTilesets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesets)
{
    ui->setupUi(this);

    ui->btNew->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btRemove->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveUp->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveDown->setStyleSheet(App::getStyles()->get("button_click"));

    connect(App::getState(), &AppState::onSelectedSceneIDChanged, this, [&](int sceneID)
    {
        (void)sceneID;
        updateTilesetsWidget();

        if (!App::getState()->filteredTilesets()->isEmpty())
            ui->listTilesets->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onFilteredTilesetsChanged, this, [&](QList<Tileset*> const * tilesets)
    {
        auto oldTileset = App::getState()->selectedTileset();
        auto oldID = oldTileset == nullptr ? 0 : oldTileset->id;
        auto oldPosition = ui->listTilesets->currentRow();

        updateTilesetsWidget();

        if (tilesets->isEmpty())
            return;

        for (qsizetype i=0;i!=tilesets->count();++i)
        {
            if (tilesets->at(i)->id == oldID)
            {
                ui->listTilesets->setCurrentRow(i);
                return;
            }
        }

        if (oldPosition >= 0 && oldPosition < tilesets->count())
            ui->listTilesets->setCurrentRow(oldPosition);
        else
            ui->listTilesets->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onTilesetInserted, this, [&](QList<Tileset*> const * value, int const position)
    {
        (void)value;
        updateTilesetsWidget();
        ui->listTilesets->setCurrentRow(position);
    });

    connect(App::getState(), &AppState::onTilesetRemoved, this, [&](QList<Tileset*> const * value, int const position)
    {
        updateTilesetsWidget();
        if (value->size() != 0)
            ui->listTilesets->setCurrentRow(position >= value->size() ? value->size()-1 : position);
    });

    connect(App::getState(), &AppState::onTilesetMoved, this, [&](QList<Tileset*> const * value, int const oldPosition, int const newPosition)
    {
        (void)value;
        (void)oldPosition;
        updateTilesetsWidget();
        ui->listTilesets->setCurrentRow(newPosition);
    });

    connect(ui->btNew, &QPushButton::clicked, this, [&]()
    {
        auto sceneID = App::getState()->selectedSceneID();
        auto ts = new Tileset();

        ts->id = ++App::getState()->project()->lastTilesetID;
        ts->sceneId = sceneID==-1?0:sceneID;
        ts->name = "Noname";
        ts->gridW = 10;
        ts->gridH = 10;
        ts->bgColor = QColor::fromRgb(0,0,0);

        int const position = ui->listTilesets->currentRow() < 0 ? 0 : ui->listTilesets->currentRow()+1;

        App::getState()->insertTileset(position, ts);
    });

    connect(ui->btRemove, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
            App::getState()->removeTileset(ui->listTilesets->currentRow());
    });

    connect(ui->btMoveUp, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
            App::getState()->moveDownTileset(ui->listTilesets->currentRow());
    });

    connect(ui->btMoveDown, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
            App::getState()->moveUpTileset(ui->listTilesets->currentRow());
    });

    connect(ui->listTilesets, &QListWidget::currentRowChanged, this, [&](int position)
    {
        auto tilesets = App::getState()->filteredTilesets();

        if (position < 0 || position >= tilesets->size())
            App::getState()->setSelectedTileset(nullptr);
        else
            App::getState()->setSelectedTileset(tilesets->at(position));
    });

    updateTilesetsWidget();
    if (!App::getState()->filteredTilesets()->isEmpty())
        ui->listTilesets->setCurrentRow(0);

}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

void FragmentTilesets::updateTilesetsWidget()
{
    ui->listTilesets->clear();

    for (auto ts : *App::getState()->filteredTilesets())
    {
        QString number = QString::number(ts->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number, ts->name);
        ui->listTilesets->addItem(newText);
    }
}


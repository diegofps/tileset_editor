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

    connect(App::getState(), &AppState::onSelectedScenePosChanged, this, [&](int scenePos) {
        filterTilesets(scenePos, App::getState()->projectTilesets());
        updateTilesetsWidget();

        if (!_tilesets.isEmpty())
            ui->listTilesets->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onProjectTilesetsChanged, this, [&](QList<Tileset*> const * value)
    {
        auto oldTileset = App::getState()->selectedTileset();
        auto oldID = oldTileset == nullptr ? 0 : oldTileset->id;
        auto oldPosition = ui->listTilesets->currentRow();
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());

        filterTilesets(sceneID, value);
        updateTilesetsWidget();

        if (_tilesets.isEmpty())
            return;

        for (qsizetype i=0;i!=_tilesets.count();++i)
            if (_tilesets[i]->id == oldID)
            {
                ui->listTilesets->setCurrentRow(i);
                return;
            }

        if (oldPosition >= 0 && oldPosition < _tilesets.count())
            ui->listTilesets->setCurrentRow(oldPosition);
        else
            ui->listTilesets->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onProjectTilesetsInserted, this, [&](QList<Tileset*> const * value, int const position)
    {
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());
        filterTilesets(sceneID, value);
        updateTilesetsWidget();
        ui->listTilesets->setCurrentRow(position);
    });

    connect(App::getState(), &AppState::onProjectTilesetsRemoved, this, [&](QList<Tileset*> const * value, int const position)
    {
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());
        filterTilesets(sceneID, value);
        updateTilesetsWidget();
        if (value->size() != 0)
            ui->listTilesets->setCurrentRow(position >= value->size() ? value->size()-1 : position);
    });

    connect(App::getState(), &AppState::onProjectTilesetsMoved, this, [&](QList<Tileset*> const * value, int const oldPosition, int const newPosition)
    {
        (void)oldPosition;
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());
        filterTilesets(sceneID, value);
        updateTilesetsWidget();
        ui->listTilesets->setCurrentRow(newPosition);
    });

    connect(ui->btNew, &QPushButton::clicked, this, [&]()
    {
        auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());

        auto ts = new Tileset();
        ts->id = ++App::getState()->project()->lastTilesetID;
        ts->sceneId = sceneID==-1?0:sceneID;
        ts->name = "Noname";
        ts->gridW = 10;
        ts->gridH = 10;
        ts->bgColor = QColor::fromRgb(0,0,0);

        int const position = ui->listTilesets->currentRow() < 0 ? 0 : ui->listTilesets->currentRow()+1;
        App::getState()->insertProjectTileset(position, ts);
        App::getState()->setProjectHasChanges(true);
    });

    connect(ui->btRemove, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
        {
            App::getState()->removeProjectTileset(ui->listTilesets->currentRow());
            App::getState()->setProjectHasChanges(true);
        }
    });

    connect(ui->btMoveUp, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
        {
            App::getState()->moveDownProjectTileset(ui->listTilesets->currentRow());
            App::getState()->setProjectHasChanges(true);
        }
    });

    connect(ui->btMoveDown, &QPushButton::clicked, this, [&]()
    {
        if (ui->listTilesets->currentRow() >= 0)
        {
            App::getState()->moveUpProjectTileset(ui->listTilesets->currentRow());
            App::getState()->setProjectHasChanges(true);
        }
    });

    connect(ui->listTilesets, &QListWidget::currentRowChanged, this, [&](int position) {

        if (position < 0 || position >= _tilesets.size())
            App::getState()->setSelectedTileset(nullptr);
        else
            App::getState()->setSelectedTileset(_tilesets[position]);
    });

    connect(App::getState(), &AppState::onSelectedTilesetChanged, this, [&](Tileset * ts) {
        if (ui->listTilesets->currentRow() < 0 || ts == nullptr)
            return;

        qDebug() << "Item changed, name=" << ts->name;

        auto item = ui->listTilesets->item(ui->listTilesets->currentRow());

        QString number = QString::number(ts->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number, ts->name);

        if (item->text() != newText)
            item->setText(newText);
    });

    auto tilesets = App::getState()->projectTilesets();
    auto sceneID = App::getState()->scenePos2ID(App::getState()->selectedScenePos());
    filterTilesets(sceneID, tilesets);
    updateTilesetsWidget();
    if (tilesets->size() != 0)
        ui->listTilesets->setCurrentRow(0);

}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

void FragmentTilesets::filterTilesets(int sceneID, const QList<Tileset *> *value)
{
    _tilesets.clear();

    if (value == nullptr)
        return;

    for (auto ts : *value)
    {
        if (sceneID != -1)
            if (sceneID != ts->sceneId)
                continue;

        _tilesets.append(ts);
    }
}

void FragmentTilesets::updateTilesetsWidget()
{
    ui->listTilesets->clear();

    for (auto ts : _tilesets)
    {
        QString number = QString::number(ts->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number, ts->name);
        ui->listTilesets->addItem(newText);
    }
}


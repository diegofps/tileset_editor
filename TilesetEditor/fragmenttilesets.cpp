#include "fragmenttilesets.h"
#include "ui_fragmenttilesets.h"

#include "app.h"

FragmentTilesets::FragmentTilesets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesets)
{
    ui->setupUi(this);

    loadTilesets(App::getState()->projectTilesets());

    ui->btNew->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btRemove->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveUp->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveDown->setStyleSheet(App::getStyles()->get("button_click"));

    connect(App::getState(), &AppState::onProjectTilesetsChanged, this, [&](QList<Tileset*> const * value)
    {
        loadTilesets(value);
    });

    connect(ui->btNew, &QPushButton::clicked, this, [&]()
    {
        auto ts = new Tileset();
        ts->id = ++App::getState()->project()->lastTilesetID;
        ts->name = "Noname";
        ts->gridW = 10;
        ts->gridH = 10;
        ts->bgColor = QColor::fromRgb(0,0,0);

        auto tilesets = App::getState()->projectTilesets();
        tilesets->append(ts);
        App::getState()->setProjectTilesets(tilesets);
    });

    connect(ui->btRemove, &QPushButton::clicked, this, [&]()
    {
        // TODO
    });

    connect(ui->btMoveUp, &QPushButton::clicked, this, [&]()
    {
        // TODO
    });

    connect(ui->btMoveDown, &QPushButton::clicked, this, [&]()
    {
        // TODO
    });

}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

void FragmentTilesets::loadTilesets(QList<Tileset*> const * value)
{
    ui->listTilesets->clear();

    for (auto ts : *value)
        ui->listTilesets->addItem(QString("%1: %2").arg(ts->id).arg(ts->name));

    if (ui->listTilesets->currentRow() < 0 && !value->isEmpty())
        ui->listTilesets->setCurrentRow(0);
}


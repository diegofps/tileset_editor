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

    connect(App::getState(), &AppState::onProjectTilesetsChanged, this, [&](QList<Tileset*> const * value)
    {
        loadTilesets(value);
        if (value != nullptr && ui->listTilesets->currentRow() < 0 && !value->isEmpty())
            ui->listTilesets->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onProjectTilesetsInserted, this, [&](QList<Tileset*> const * value, int const position)
    {
        loadTilesets(value);
        ui->listTilesets->setCurrentRow(position);
    });

    connect(App::getState(), &AppState::onProjectTilesetsRemoved, this, [&](QList<Tileset*> const * value, int const position)
    {
        loadTilesets(value);
        if (value->size() != 0)
            ui->listTilesets->setCurrentRow(position >= value->size() ? value->size()-1 : position);
    });

    connect(App::getState(), &AppState::onProjectTilesetsMoved, this, [&](QList<Tileset*> const * value, int const oldPosition, int const newPosition)
    {
        (void)oldPosition;
        loadTilesets(value);
        ui->listTilesets->setCurrentRow(newPosition);
    });

    connect(ui->btNew, &QPushButton::clicked, this, [&]()
    {
        auto ts = new Tileset();
        ts->id = ++App::getState()->project()->lastTilesetID;
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

        auto tilesets = App::getState()->projectTilesets();

        if (position < 0 || position >= tilesets->size())
            App::getState()->setTilesetsSelectedItem(nullptr);
        else
            App::getState()->setTilesetsSelectedItem(tilesets->at(position));
    });

    connect(App::getState(), &AppState::onTilesetsSelectedItemChanged, this, [&](Tileset * ts) {
        if (ui->listTilesets->currentRow() < 0 || ts == nullptr)
            return;

        qDebug() << "Item changed, name=" << ts->name;

        auto item = ui->listTilesets->item(ui->listTilesets->currentRow());

        QString number = QString::number(ts->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number).arg(ts->name);

        if (item->text() != newText)
            item->setText(newText);
    });

    auto tilesets = App::getState()->projectTilesets();
    loadTilesets(tilesets);
    if (tilesets->size() != 0)
        ui->listTilesets->setCurrentRow(0);

}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

void FragmentTilesets::loadTilesets(QList<Tileset*> const * value)
{
    ui->listTilesets->clear();

    if (value == nullptr)
        return;

    for (auto ts : *value)
    {
        QString number = QString::number(ts->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number).arg(ts->name);
        ui->listTilesets->addItem(newText);
    }
}


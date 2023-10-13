#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"

void FragmentTiles::styleButtons(QString const & value)
{
    if (value == "all")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == "unlinked")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btAll->update();
    ui->btUnlinked->update();
}

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);

    styleButtons(App::getState()->tilesShow());

    connect(ui->btAll, &QPushButton::clicked, this, [&](){ App::getState()->setTilesShow("all"); });
    connect(ui->btUnlinked, &QPushButton::clicked, this, [&](){ App::getState()->setTilesShow("unlinked"); });
    connect(App::getState(), &AppState::onTilesShowChanged, this, [&](QString const & value) { styleButtons(value); });
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

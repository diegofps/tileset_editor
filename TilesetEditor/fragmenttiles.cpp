#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);

    if (App::getState().tilesShow() == "all")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (App::getState().tilesShow() == "unlinked")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    connect(ui->btAll, &QPushButton::clicked, [&](){ App::getState().setTilesShow("all"); });
    connect(ui->btUnlinked, &QPushButton::clicked, [&](){ App::getState().setTilesShow("unlinked"); });

    connect(&App::getState(), &AppState::onTilesShowChanged, [&](QString const & value) {
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
    });
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

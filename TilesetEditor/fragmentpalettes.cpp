#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

#include "app.h"

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);

    if (App::getState()->palettesShow() == "all")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (App::getState()->palettesShow() == "used")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    connect(ui->btAll, &QPushButton::clicked, this, [&](){ App::getState()->setPalettesShow("all"); });
    connect(ui->btUsedWithTile, &QPushButton::clicked, this, [&](){ App::getState()->setPalettesShow("used"); });

    connect(App::getState(), &AppState::onPalettesShowChanged, this, [&](QString const & value) {
        if (value == "all")
        {
            ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
            ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
        }
        else if (value == "used")
        {
            ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
            ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_checked"));
        }
        else
        {
            ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
            ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
        }
    });
}

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

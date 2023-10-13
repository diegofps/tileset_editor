#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

#include "app.h"

void FragmentPalettes::styleButtons(QString const & value)
{
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

    ui->btAll->update();
    ui->btUsedWithTile->update();
}

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);

    styleButtons(App::getState()->palettesShow());

    connect(ui->btAll, &QPushButton::clicked, this, [&](){ App::getState()->setPalettesShow("all"); });
    connect(ui->btUsedWithTile, &QPushButton::clicked, this, [&](){ App::getState()->setPalettesShow("used"); });
    connect(App::getState(), &AppState::onPalettesShowChanged, this, [&](QString const & value) { styleButtons(value); });
}

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

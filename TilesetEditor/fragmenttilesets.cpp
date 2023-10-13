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

    connect(ui->btNew, &QPushButton::clicked, [&](){
        // TODO
    });

    connect(ui->btRemove, &QPushButton::clicked, [&](){
        // TODO
    });
}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

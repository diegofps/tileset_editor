#include "fragmenttilesets.h"
#include "ui_fragmenttilesets.h"

FragmentTilesets::FragmentTilesets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesets)
{
    ui->setupUi(this);
}

FragmentTilesets::~FragmentTilesets()
{
    delete ui;
}

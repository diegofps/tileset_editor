#include "fragmenttilesetproperties.h"
#include "ui_fragmenttilesetproperties.h"

FragmentTilesetProperties::FragmentTilesetProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesetProperties)
{
    ui->setupUi(this);
}

FragmentTilesetProperties::~FragmentTilesetProperties()
{
    delete ui;
}

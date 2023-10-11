#include "fragmenttiles.h"
#include "ui_fragmenttiles.h"

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

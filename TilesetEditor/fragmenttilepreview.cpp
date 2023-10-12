#include "fragmenttilepreview.h"
#include "ui_fragmenttilepreview.h"

FragmentTilePreview::FragmentTilePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilePreview)
{
    ui->setupUi(this);

    ui->btOriginal->setChecked(true);
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

#include "fragmenttilepreview.h"
#include "ui_fragmenttilepreview.h"

FragmentTilePreview::FragmentTilePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilePreview)
{
    ui->setupUi(this);
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

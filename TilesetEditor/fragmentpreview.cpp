#include "fragmentpreview.h"
#include "ui_fragmentpreview.h"

FragmentPreview::FragmentPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPreview)
{
    ui->setupUi(this);
}

FragmentPreview::~FragmentPreview()
{
    delete ui;
}

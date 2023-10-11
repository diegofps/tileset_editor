#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);
}

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

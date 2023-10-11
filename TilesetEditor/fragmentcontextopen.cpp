#include "fragmentcontextopen.h"
#include "ui_fragmentcontextopen.h"

FragmentContextOpen::FragmentContextOpen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentContextOpen)
{
    ui->setupUi(this);
}

FragmentContextOpen::~FragmentContextOpen()
{
    delete ui;
}

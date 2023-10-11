#include "fragmentcontextclosed.h"
#include "ui_fragmentcontextclosed.h"

FragmentContextClosed::FragmentContextClosed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentContextClosed)
{
    ui->setupUi(this);
}

FragmentContextClosed::~FragmentContextClosed()
{
    delete ui;
}

#include "fragmentreferences.h"
#include "ui_fragmentreferences.h"

FragmentReferences::FragmentReferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentReferences)
{
    ui->setupUi(this);
}

FragmentReferences::~FragmentReferences()
{
    delete ui;
}

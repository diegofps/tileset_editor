#include "fragmentreferencestoolbar.h"
#include "ui_fragmentreferencestoolbar.h"

FragmentReferencesToolbar::FragmentReferencesToolbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentReferencesToolbar)
{
    ui->setupUi(this);
}

FragmentReferencesToolbar::~FragmentReferencesToolbar()
{
    delete ui;
}

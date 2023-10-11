#include "fragmenteditor.h"
#include "ui_fragmenteditor.h"

FragmentEditor::FragmentEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentEditor)
{
    ui->setupUi(this);
}

FragmentEditor::~FragmentEditor()
{
    delete ui;
}

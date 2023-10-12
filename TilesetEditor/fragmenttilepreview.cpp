#include "fragmenttilepreview.h"
#include "ui_fragmenttilepreview.h"

#include "app.h"

FragmentTilePreview::FragmentTilePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilePreview)
{
    ui->setupUi(this);

    if (App::getState().tilePreviewShow() == "original")
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (App::getState().tilePreviewShow() == "hd")
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    connect(ui->btOriginal, &QPushButton::clicked, [&](){ App::getState().setTilePreviewShow("original"); });
    connect(ui->btHD, &QPushButton::clicked, [&](){ App::getState().setTilePreviewShow("hd"); });

    connect(&App::getState(), &AppState::onTilePreviewShowChanged, [&](QString const & value) {
        if (value == "original")
        {
            ui->btOriginal->setStyleSheet(App::getStyles()->get("button_checked"));
            ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
        }
        else if (value == "hd")
        {
            ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
            ui->btHD->setStyleSheet(App::getStyles()->get("button_checked"));
        }
        else
        {
            ui->btOriginal->setStyleSheet(App::getStyles()->get("button_unchecked"));
            ui->btHD->setStyleSheet(App::getStyles()->get("button_unchecked"));
        }
    });
}

FragmentTilePreview::~FragmentTilePreview()
{
    delete ui;
}

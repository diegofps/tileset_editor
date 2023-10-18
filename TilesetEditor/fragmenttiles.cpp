#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgetpreview.h"
#include "qverticalscrollarea.h"

void FragmentTiles::styleButtons(QString const & value)
{
    if (value == "all")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == "unlinked")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUnlinked->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btAll->update();
    ui->btUnlinked->update();
}

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);

    styleButtons(App::getState()->tilesShow());

    connect(ui->btAll, &QPushButton::clicked, this, [&](){ App::getState()->setTilesShow("all"); });
    connect(ui->btUnlinked, &QPushButton::clicked, this, [&](){ App::getState()->setTilesShow("unlinked"); });
    connect(App::getState(), &AppState::onTilesShowChanged, this, [&](QString const & value) { styleButtons(value); });

    auto wPreview = new WidgetPreview(this);
    wPreview->setMinimumHeight(1000);

//    auto layout = new QVBoxLayout();
//    layout->addWidget(wPreview);

    auto vScrollArea = new QVerticalScrollArea(wPreview, this);
//    vScrollArea->setLayout(layout);

    auto layout2 = new QVBoxLayout();
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

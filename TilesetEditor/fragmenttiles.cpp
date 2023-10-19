#include "fragmenttiles.h"
#include "qstyle.h"
#include "ui_fragmenttiles.h"
#include "app.h"
#include "widgetpreview.h"
#include "qverticalscrollarea.h"

void FragmentTiles::styleButton(QPushButton * btn, bool value)
{
    btn->setStyleSheet(App::getStyles()->get(value?"button_checked":"button_unchecked"));
    btn->update();
}

FragmentTiles::FragmentTiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTiles)
{
    ui->setupUi(this);

    auto filter = App::getState()->tilesFilter();

    styleButton(ui->btBackground, filter->usedInBackground);
    styleButton(ui->btUnlinked, filter->isUnlinked);
    styleButton(ui->btSprite, filter->usedInSprite);

    connect(ui->btSprite, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->usedInSprite = !filter->usedInSprite;
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btBackground, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->usedInBackground = !filter->usedInBackground;
        App::getState()->setTilesFilter(filter);
    });

    connect(ui->btUnlinked, &QPushButton::clicked, this, [&]()
    {
        auto filter = App::getState()->tilesFilter();
        filter->isUnlinked = !filter->isUnlinked;
        App::getState()->setTilesFilter(filter);
    });

    connect(App::getState(), &AppState::onTilesFilterChanged, this, [&](TilesFilter * filter)
    {
        styleButton(ui->btBackground, filter->usedInBackground);
        styleButton(ui->btSprite, filter->usedInSprite);
        styleButton(ui->btUnlinked, filter->isUnlinked);
    });

    auto wPreview = new WidgetPreview(this);
    wPreview->setMinimumHeight(1000);

    auto vScrollArea = new QVerticalScrollArea(wPreview, this);

    auto layout2 = new QVBoxLayout();
    layout2->addWidget(vScrollArea);

    ui->listFrame->setLayout(layout2);
}

FragmentTiles::~FragmentTiles()
{
    delete ui;
}

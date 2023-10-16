#include "fragmentpalettes.h"
#include "ui_fragmentpalettes.h"

#include "app.h"
#include "widgetitempalette.h"

void FragmentPalettes::styleButtons(QString const & value)
{
    if (value == "all")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == "used")
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btAll->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btUsedWithTile->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btAll->update();
    ui->btUsedWithTile->update();
}

FragmentPalettes::FragmentPalettes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPalettes)
{
    ui->setupUi(this);

    styleButtons(App::getState()->palettesShow());

    connect(ui->btAll, &QPushButton::clicked, this, [&]() { App::getState()->setPalettesShow("all"); });
    connect(ui->btUsedWithTile, &QPushButton::clicked, this, [&]() { App::getState()->setPalettesShow("used"); });
    connect(App::getState(), &AppState::onPalettesShowChanged, this, [&](QString const & value) { styleButtons(value); });
    connect(App::getState(), &AppState::onProjectPalettesChanged, this, [&](QList<Palette*> const * value) { loadPalettes(value); });

    connect(ui->listPalettes, &QListWidget::currentRowChanged, this, [&](int position){

        auto palettes = App::getState()->projectPalettes();

        if (position < 0 || position >= palettes->size())
            App::getState()->setPalettesSelectedItem(nullptr);
        else
            App::getState()->setPalettesSelectedItem(palettes->at(position));
    });

    auto value = App::getState()->projectPalettes();
    loadPalettes(value);
    if (value->size() != 0)
        ui->listPalettes->setCurrentRow(0);
}

void FragmentPalettes::loadPalettes(QList<Palette*> const * value)
{
    ui->listPalettes->clear();

    if (value == nullptr)
        return;

    for (auto p : *value)
    {
        auto itemWidget = new WidgetItemPalette(this);
        itemWidget->setPalette(p);

        auto item = new QListWidgetItem();
        item->setSizeHint(itemWidget->sizeHint());

        ui->listPalettes->addItem(item);
        ui->listPalettes->setItemWidget(item, itemWidget);
    }
}

FragmentPalettes::~FragmentPalettes()
{
    delete ui;
}

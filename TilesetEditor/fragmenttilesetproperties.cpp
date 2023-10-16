#include "fragmenttilesetproperties.h"
#include "ui_fragmenttilesetproperties.h"

#include "app.h"

#include <QColorDialog>

FragmentTilesetProperties::FragmentTilesetProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesetProperties)
{
    ui->setupUi(this);

    connect(ui->leName, &QLineEdit::textChanged, this, [&](QString const & text) {
        auto ts = App::getState()->tilesetSelectedItem();
        if (ts != nullptr && ts->name != text) {
            ts->name = text;
            App::getState()->setTilesetsSelectedItem(ts);
        }
    });

    connect(ui->sbGridW, &QSpinBox::valueChanged, this, [&](int value) {
        auto ts = App::getState()->tilesetSelectedItem();
        if (ts != nullptr && ts->gridW != value) {
            ts->gridW = value;
            App::getState()->setTilesetsSelectedItem(ts);
        }

        ui->leWidthOriginal->setText(QString::number(value*8));
        ui->leWidthHD->setText(QString::number(value*8*16));
    });

    connect(ui->sbGridH, &QSpinBox::valueChanged, this, [&](int value) {
        auto ts = App::getState()->tilesetSelectedItem();
        if (ts != nullptr && ts->gridH != value) {
            ts->gridH = value;
            App::getState()->setTilesetsSelectedItem(ts);
        }

        ui->leHeightOriginal->setText(QString::number(value*8));
        ui->leHeightHD->setText(QString::number(value*8*16));
    });

    connect(ui->btColor, &QPushButton::clicked, this, [&](bool) {
        auto ts = App::getState()->tilesetSelectedItem();
        if (ts == nullptr)
            return;

        QColorDialog dialog;
        dialog.setCurrentColor(ts->bgColor);

        if (dialog.exec() && ts->bgColor != dialog.selectedColor()) {
            ts->bgColor = dialog.selectedColor();
            App::getState()->setTilesetsSelectedItem(ts);
        }
    });

    connect(App::getState(), &AppState::onTilesetsSelectedItemChanged, this, &FragmentTilesetProperties::loadTileset);

    loadTileset(App::getState()->tilesetSelectedItem());
}

void FragmentTilesetProperties::loadTileset(Tileset * ts)
{
    if (ts == nullptr)
    {
        qDebug() << "Changed to nullptr";

        ui->leName->setEnabled(false);
        ui->sbGridW->setEnabled(false);
        ui->sbGridH->setEnabled(false);
        ui->btColor->setEnabled(false);

        ui->leID->setText("");
        ui->leName->setText("");
        ui->sbGridW->setValue(0);
        ui->sbGridH->setValue(0);
        ui->btColor->setStyleSheet("background-color: #000000");
    }
    else
    {
        qDebug() << "Changed to " << ts->id;

        ui->leName->setEnabled(true);
        ui->sbGridW->setEnabled(true);
        ui->sbGridH->setEnabled(true);
        ui->btColor->setEnabled(true);

        ui->leID->setText(QString::number(ts->id));
        ui->leName->setText(ts->name);
        ui->sbGridW->setValue(ts->gridW);
        ui->sbGridH->setValue(ts->gridH);
        ui->btColor->setStyleSheet("background-color: " + ts->bgColor.name(QColor::HexRgb));
    }
}

FragmentTilesetProperties::~FragmentTilesetProperties()
{
    delete ui;
}

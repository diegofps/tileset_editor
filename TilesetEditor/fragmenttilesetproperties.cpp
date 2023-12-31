#include "fragmenttilesetproperties.h"
#include "ui_fragmenttilesetproperties.h"

#include "app.h"

#include <QColorDialog>

QString COLOR_STYLE = "border:0px; padding:3px 8px; border-radius:10px; background-color: ";

FragmentTilesetProperties::FragmentTilesetProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentTilesetProperties)
{
    ui->setupUi(this);

    connect(ui->leName, &QLineEdit::textChanged, this, [&](QString const & text) {
        auto ts = App::getState()->selectedTileset();
        if (ts != nullptr && ts->name != text) {
            ts->name = text;
            App::getState()->setSelectedTileset(ts);
            App::getState()->setProjectHasChanges(true);
        }
    });

    connect(ui->sbGridW, &QSpinBox::valueChanged, this, [&](int value) {
        auto ts = App::getState()->selectedTileset();
        if (ts != nullptr && ts->gridW != value) {
            ts->gridW = value;
            App::getState()->setSelectedTileset(ts);
            App::getState()->setProjectHasChanges(true);
        }

        ui->leWidthOriginal->setText(QString::number(value*8));
        ui->leWidthHD->setText(QString::number(value*8*16));
    });

    connect(ui->sbGridH, &QSpinBox::valueChanged, this, [&](int value) {
        auto ts = App::getState()->selectedTileset();
        if (ts != nullptr && ts->gridH != value) {
            ts->gridH = value;
            App::getState()->setSelectedTileset(ts);
            App::getState()->setProjectHasChanges(true);
        }

        ui->leHeightOriginal->setText(QString::number(value*8));
        ui->leHeightHD->setText(QString::number(value*8*16));
    });

    connect(ui->btColor, &QPushButton::clicked, this, [&](bool) {
        auto ts = App::getState()->selectedTileset();
        if (ts == nullptr)
            return;

        QColorDialog dialog;
        dialog.setCurrentColor(ts->bgColor);

        if (dialog.exec() && ts->bgColor != dialog.selectedColor()) {
            ts->bgColor = dialog.selectedColor();
            App::getState()->setSelectedTileset(ts);
            App::getState()->setProjectHasChanges(true);
        }
    });

    connect(App::getState(), &AppState::onSelectedTilesetChanged, this, &FragmentTilesetProperties::loadTileset);

    loadTileset(App::getState()->selectedTileset());
}

void FragmentTilesetProperties::loadTileset(Tileset * ts)
{
    if (ts == nullptr)
    {
//        qDebug() << "Changed to nullptr";

        ui->leName->setEnabled(false);
        ui->sbGridW->setEnabled(false);
        ui->sbGridH->setEnabled(false);
        ui->btColor->setEnabled(false);

        ui->leID->setText("");
        ui->leName->setText("");
        ui->sbGridW->setValue(0);
        ui->sbGridH->setValue(0);
        ui->btColor->setStyleSheet(COLOR_STYLE + "#000000");
    }
    else
    {
//        qDebug() << "Changed to " << ts->id;

        ui->leName->setEnabled(true);
        ui->sbGridW->setEnabled(true);
        ui->sbGridH->setEnabled(true);
        ui->btColor->setEnabled(true);

        ui->leID->setText(QString::number(ts->id));
        ui->leName->setText(ts->name);
        ui->sbGridW->setValue(ts->gridW);
        ui->sbGridH->setValue(ts->gridH);
        ui->btColor->setStyleSheet(COLOR_STYLE + ts->bgColor.name(QColor::HexRgb));
    }
}

FragmentTilesetProperties::~FragmentTilesetProperties()
{
    delete ui;
}

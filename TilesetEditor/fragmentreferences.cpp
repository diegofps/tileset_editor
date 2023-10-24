#include "fragmentreferences.h"
#include "ui_fragmentreferences.h"

#include "app.h"

void FragmentReferences::styleScreenshotButtons(ReferenceMode const value)
{
    auto checked = App::getStyles()->get("button_checked");
    auto unchecked = App::getStyles()->get("button_unchecked");

    ui->btRef1->setStyleSheet(value==REF_1?checked:unchecked);
    ui->btRef10->setStyleSheet(value==REF_10?checked:unchecked);
    ui->btRef100->setStyleSheet(value==REF_100?checked:unchecked);
    ui->btRef1000->setStyleSheet(value==REF_1000?checked:unchecked);

    ui->btRefNN->setStyleSheet(value==REF_NN?checked:unchecked);
    ui->btRefNF->setStyleSheet(value==REF_NF?checked:unchecked);
    ui->btRefFN->setStyleSheet(value==REF_FN?checked:unchecked);
    ui->btRefFF->setStyleSheet(value==REF_FF?checked:unchecked);

    ui->btRef1->update();
    ui->btRef10->update();
    ui->btRef100->update();
    ui->btRef1000->update();

    ui->btRefNN->update();
    ui->btRefNF->update();
    ui->btRefFN->update();
    ui->btRefFF->update();
}

void FragmentReferences::styleHighlightPositionButton(bool const value)
{
    ui->btShowGrid->setStyleSheet(App::getStyles()->get(value ? "button_checked" : "button_unchecked"));
    ui->btShowGrid->update();
}

FragmentReferences::FragmentReferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentReferences)
{
    ui->setupUi(this);

    styleScreenshotButtons(App::getState()->referenceMode());
    styleHighlightPositionButton(App::getState()->referenceHighlightPosition());

    connect(ui->btRef1, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_1); });
    connect(ui->btRef10, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_10); });
    connect(ui->btRef100, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_100); });
    connect(ui->btRef1000, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_1000); });

    connect(ui->btRefNN, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_NN); });
    connect(ui->btRefNF, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_NF); });
    connect(ui->btRefFN, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_FN); });
    connect(ui->btRefFF, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_FF); });

    connect(ui->btShowGrid, &QPushButton::clicked, this, [&]() {
        App::getState()->setReferenceHighlightPosition(!App::getState()->referenceHighlightPosition());
    });

    connect(App::getState(), &AppState::onReferenceModeChanged, this, [&](ReferenceMode const value) { styleScreenshotButtons(value); });
    connect(App::getState(), &AppState::onReferenceHighlightPositionChanged, this, [&](bool const value) { styleHighlightPositionButton(value); });
}

FragmentReferences::~FragmentReferences()
{
    delete ui;
}

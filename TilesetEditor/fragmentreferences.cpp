#include "fragmentreferences.h"
#include "ui_fragmentreferences.h"

#include "app.h"

void FragmentReferences::styleScreenshotButtons(int const value)
{
    if (value == 0)
    {
        ui->btRef1->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btRef10->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef100->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == 1)
    {
        ui->btRef1->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef10->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btRef100->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == 2)
    {
        ui->btRef1->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef10->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef100->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btRef1->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef10->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btRef100->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btRef1->update();
    ui->btRef10->update();
    ui->btRef100->update();
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

    styleScreenshotButtons(App::getState()->referenceScreenshot());
    styleHighlightPositionButton(App::getState()->referenceHighlightPosition());

    connect(ui->btRef1, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceScreenshot(0); });
    connect(ui->btRef10, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceScreenshot(1); });
    connect(ui->btRef100, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceScreenshot(2); });

    connect(ui->btShowGrid, &QPushButton::clicked, this, [&]() {
        App::getState()->setReferenceHighlightPosition(!App::getState()->referenceHighlightPosition());
    });

    connect(App::getState(), &AppState::onReferenceScreenshotChanged, this, [&](int const value) { styleScreenshotButtons(value); });
    connect(App::getState(), &AppState::onReferenceHighlightPositionChanged, this, [&](bool const value) { styleHighlightPositionButton(value); });
}

FragmentReferences::~FragmentReferences()
{
    delete ui;
}

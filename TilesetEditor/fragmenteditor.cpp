#include "app.h"
#include "appstate.h"
#include "fragmenteditor.h"
#include "ui_fragmenteditor.h"

void FragmentEditor::styleToolButtons(EditorTool const value)
{
    if (value == PENCIL)
    {
        ui->btPencil->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btEraser->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btLinker->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == ERASER)
    {
        ui->btPencil->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btEraser->setStyleSheet(App::getStyles()->get("button_checked"));
        ui->btLinker->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == LINKER)
    {
        ui->btPencil->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btEraser->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btLinker->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        ui->btPencil->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btEraser->setStyleSheet(App::getStyles()->get("button_unchecked"));
        ui->btLinker->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    ui->btPencil->update();
    ui->btEraser->update();
    ui->btLinker->update();
}

void FragmentEditor::styleButton(bool const value, QPushButton * const button)
{
    button->setStyleSheet(App::getStyles()->get(value ? "button_checked" : "button_unchecked"));
    button->update();
}

FragmentEditor::FragmentEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentEditor)
{
    ui->setupUi(this);

//    ui->btPencil->setToolTip("Draws the selected Tile to the Tileset");
//    ui->btEraser->setToolTip("Erase Tiles from the Tileset");
//    ui->btLinker->setToolTip("Mark Tiles that link their original image to its HD version");

    styleToolButtons(App::getState()->editorTool());
//    ui->btShowNext->setStyleSheet(App::getStyles()->get("button_click"));
    styleButton(App::getState()->editorShowLinkedTiles(), ui->btShowLinkedTiles);
    styleButton(App::getState()->editorShowUnlinkedTiles(), ui->btShowUnlinkedTiles);
    styleButton(App::getState()->editorShowGrid(), ui->btShowGrid);

    connect(ui->btPencil, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(PENCIL); });
    connect(ui->btEraser, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(ERASER); });
    connect(ui->btLinker, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(LINKER); });

//    connect(ui->btShowNext, &QPushButton::clicked, this, [&]() {
//        // TODO: Show next tile appearance
//    });

    connect(ui->btShowLinkedTiles, &QPushButton::clicked, this, [&]() {
        App::getState()->setEditorShowLinkedTiles(!App::getState()->editorShowLinkedTiles());
    });

    connect(ui->btShowUnlinkedTiles, &QPushButton::clicked, this, [&]() {
        App::getState()->setEditorShowUnlinkedTiles(!App::getState()->editorShowUnlinkedTiles());
    });

    connect(ui->btShowGrid, &QPushButton::clicked, this, [&]() {
        App::getState()->setEditorShowGrid(!App::getState()->editorShowGrid());
    });

    // Listen to signals

    connect(App::getState(), &AppState::onEditorToolChanged, this, [&](EditorTool value) { styleToolButtons(value); });
    connect(App::getState(), &AppState::onEditorShowLinkedTilesChanged, this, [&](bool value) { styleButton(value, ui->btShowLinkedTiles); });
    connect(App::getState(), &AppState::onEditorShowUnlinkedTilesChanged, this, [&](bool value) { styleButton(value, ui->btShowUnlinkedTiles); });
    connect(App::getState(), &AppState::onEditorShowGridChanged, this, [&](bool value) { styleButton(value, ui->btShowGrid); });
}

FragmentEditor::~FragmentEditor()
{
    delete ui;
}

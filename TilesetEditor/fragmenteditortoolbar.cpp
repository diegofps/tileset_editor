#include "fragmenteditortoolbar.h"
#include "ui_fragmenteditortoolbar.h"

#include "app.h"

FragmentEditorToolbar::FragmentEditorToolbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentEditorToolbar)
{
    ui->setupUi(this);

    // Capture events and send commands to AppState

    connect(ui->btPencil, &QPushButton::clicked, [&]() { App::getState().setEditorTool(PENCIL); });
    connect(ui->btEraser, &QPushButton::clicked, [&]() { App::getState().setEditorTool(ERASER); });
    connect(ui->btLinker, &QPushButton::clicked, [&]() { App::getState().setEditorTool(LINKER); });

    connect(ui->btShowNext, &QPushButton::clicked, [&]() {
        // TODO: Show next tile appearance
    });

    connect(ui->btShowLinkedTiles, &QPushButton::clicked, [&]() {
        App::getState().setEditorShowLinkedTiles(!App::getState().editorShowLinkedTiles());
    });

    connect(ui->btShowUnlinkedTiles, &QPushButton::clicked, [&]() {
        App::getState().setEditorShowUnlinkedTiles(!App::getState().editorShowUnlinkedTiles());
    });

    connect(ui->btShowGrid, &QPushButton::clicked, [&]() {
        App::getState().setEditorShowGrid(!App::getState().editorShowGrid());
    });

    // Listen to signals

    connect(&App::getState(), &AppState::onEditorToolChanged, [&](EditorTool value)
    {
        if (value == PENCIL)
        {
            ui->btPencil->setChecked(true);
            ui->btEraser->setChecked(false);
            ui->btLinker->setChecked(false);
        }
        else if (value == ERASER)
        {
            ui->btPencil->setChecked(false);
            ui->btEraser->setChecked(true);
            ui->btLinker->setChecked(false);
        }
        else
        {
            ui->btPencil->setChecked(false);
            ui->btEraser->setChecked(false);
            ui->btLinker->setChecked(true);
        }
    });

    connect(&App::getState(), &AppState::onEditorShowLinkedTilesChanged, [&](bool value) {
        ui->btShowLinkedTiles->setChecked(value);
    });

    connect(&App::getState(), &AppState::onEditorShowUnlinkedTilesChanged, [&](bool value) {
        ui->btShowUnlinkedTiles->setChecked(value);
    });

    connect(&App::getState(), &AppState::onEditorShowGridChanged, [&](bool value) {
        ui->btShowGrid->setChecked(value);
    });

}

FragmentEditorToolbar::~FragmentEditorToolbar()
{
    delete ui;
}

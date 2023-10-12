#include "app.h"
#include "appstate.h"
#include "fragmenteditor.h"
#include "ui_fragmenteditor.h"

FragmentEditor::FragmentEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentEditor)
{
    ui->setupUi(this);

    connect(ui->btPencil, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(PENCIL); });
    connect(ui->btEraser, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(ERASER); });
    connect(ui->btLinker, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(LINKER); });

    connect(ui->btShowNext, &QPushButton::clicked, this, [&]() {
        // TODO: Show next tile appearance
    });

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

    connect(App::getState(), &AppState::onEditorToolChanged, this, [&](EditorTool value)
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

    connect(App::getState(), &AppState::onEditorShowLinkedTilesChanged, this, [&](bool value) {
        ui->btShowLinkedTiles->setChecked(value);
    });

    connect(App::getState(), &AppState::onEditorShowUnlinkedTilesChanged, this, [&](bool value) {
        ui->btShowUnlinkedTiles->setChecked(value);
    });

    connect(App::getState(), &AppState::onEditorShowGridChanged, this, [&](bool value) {
        ui->btShowGrid->setChecked(value);
    });

}

FragmentEditor::~FragmentEditor()
{
    delete ui;
}

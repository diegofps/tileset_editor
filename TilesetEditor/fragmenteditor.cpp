#include "app.h"
#include "appstate.h"
#include "fragmenteditor.h"
#include "ui_fragmenteditor.h"
#include "widgeteditor.h"

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

    styleToolButtons(App::getState()->editorTool());
    styleButton(App::getState()->editorShowLinkedTiles(), ui->btShowLinkedTiles);
    styleButton(App::getState()->editorShowUnlinkedTiles(), ui->btShowUnlinkedTiles);
    styleButton(App::getState()->editorShowGrid(), ui->btShowGrid);

    connect(ui->btPencil, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(PENCIL); });
    connect(ui->btEraser, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(ERASER); });
    connect(ui->btLinker, &QPushButton::clicked, this, [&]() { App::getState()->setEditorTool(LINKER); });

    connect(ui->btShowLinkedTiles, &QPushButton::clicked, this, [&]()
    {
        App::getState()->setEditorShowLinkedTiles(!App::getState()->editorShowLinkedTiles());
    });

    connect(ui->btShowUnlinkedTiles, &QPushButton::clicked, this, [&]()
    {
        App::getState()->setEditorShowUnlinkedTiles(!App::getState()->editorShowUnlinkedTiles());
    });

    connect(ui->btShowGrid, &QPushButton::clicked, this, [&]()
    {
        App::getState()->setEditorShowGrid(!App::getState()->editorShowGrid());
    });

    // Listen to signals

    connect(App::getState(), &AppState::onEditorToolChanged, this, [&](EditorTool value) { styleToolButtons(value); });
    connect(App::getState(), &AppState::onEditorShowLinkedTilesChanged, this, [&](bool value) { styleButton(value, ui->btShowLinkedTiles); });
    connect(App::getState(), &AppState::onEditorShowUnlinkedTilesChanged, this, [&](bool value) { styleButton(value, ui->btShowUnlinkedTiles); });
    connect(App::getState(), &AppState::onEditorShowGridChanged, this, [&](bool value) { styleButton(value, ui->btShowGrid); });
    connect(App::getState(), &AppState::onSelectedTilesetChanged, this, [&](Tileset * value) { updateTilesetWidget(value); });
    connect(App::getState(), &AppState::onEditorZoomChanged, this, [&](int value) { ui->widgetEditor->setZoom(value); });
    connect(App::getState(), &AppState::onMoveViewport, this, [&](int rx, int ry) { ui->widgetEditor->moveViewport(rx, ry); });
    connect(App::getState(), &AppState::onMoveViewportHome, this, [&]() { ui->widgetEditor->moveViewportHome(); });
    connect(App::getState(), &AppState::onReferenceOffsetChanged, this, [&](QPoint value) { ui->widgetEditor->setOffset(value.x(), value.y()); });
    connect(App::getState(), &AppState::onEditorRootChanged, this, [&](QPoint const value) { ui->widgetEditor->setRoot(value.x(), value.y()); });

    connect(ui->widgetEditor, &WidgetEditor::onPaintCell, this, [&](int x, int y) { App::getState()->editorPaintCellUsingSelection(x, y); });
    connect(ui->widgetEditor, &WidgetEditor::onEraseCell, this, [&](int x, int y) { App::getState()->editorEraseCell(x, y); });
    connect(ui->widgetEditor, &WidgetEditor::onColorPickCell, this, [&](int x, int y) { App::getState()->editorColorPickCell(x, y); });
    connect(ui->widgetEditor, &WidgetEditor::onLinkCell, this, [&](int x, int y) { App::getState()->editorLinkCell(x, y); });

    connect(ui->widgetEditor, &WidgetEditor::onHoverCell, this, [&](int x, int y, Cell const * cell)
    {
        if (cell == nullptr)
            ui->lbExtraInfo->setText(QString("X:%1, Y:%2").arg(x).arg(y));
        else
            ui->lbExtraInfo->setText(QString("X:%1, Y:%2, TileID:%3, PaletteID:%4, HFlip:%5, VFlip:%6, Link:%7")
                                 .arg(x)
                                 .arg(y)
                                 .arg(cell->tileID)
                                 .arg(cell->paletteID)
                                 .arg(cell->hFlip)
                                 .arg(cell->vFlip)
                                 .arg(cell->isLink));
    });

    updateTilesetWidget(App::getState()->selectedTileset());
    ui->widgetEditor->setZoom(App::getState()->editorZoom());
}

FragmentEditor::~FragmentEditor()
{
    delete ui;
}

void FragmentEditor::updateTilesetWidget(Tileset * value)
{
    if (value == nullptr)
    {
        ui->widgetEditor->setCells(nullptr);
        return;
    }

    auto root = App::getState()->editorRoot();
    auto offset = App::getState()->referenceOffset();

    ui->widgetEditor->setGridSize(value->gridW, value->gridH);
    ui->widgetEditor->setRoot(root.x(), root.y());
    ui->widgetEditor->setOffset(offset.x(), offset.y());
    ui->widgetEditor->setCells(&value->cells);
}

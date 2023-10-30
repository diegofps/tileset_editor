#include "app.h"
#include "appstate.h"
#include "fragmenteditor.h"
#include "ui_fragmenteditor.h"
#include "widgeteditor.h"

FragmentEditor::FragmentEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentEditor)
{
    ui->setupUi(this);

    // Listen to signals

    connect(App::getState(), &AppState::onSelectedTilesetChanged, this, [&](Tileset * value) { updateTilesetWidget(value); });
    connect(App::getState(), &AppState::onEditorZoomChanged, this, [&](int value) { ui->widgetEditor->setZoom(value); });
    connect(App::getState(), &AppState::onMoveViewport, this, [&](int rx, int ry) { ui->widgetEditor->moveViewport(rx, ry); });
    connect(App::getState(), &AppState::onMoveViewportHome, this, [&]() { ui->widgetEditor->moveViewportHome(); });
    connect(App::getState(), &AppState::onReferenceOffsetChanged, this, [&](QPoint value) { ui->widgetEditor->setOffset(value.x(), value.y()); });
    connect(App::getState(), &AppState::onEditorRootChanged, this, [&](QPoint const value) { ui->widgetEditor->setRoot(value.x(), value.y()); });

    connect(ui->widgetEditor, &WidgetEditor::onPaintCell, this, [&](int x, int y, bool paintTile, bool paintPalette)
        { App::getState()->editorPaintCellUsingSelection(x, y, paintTile, paintPalette); });

    connect(ui->widgetEditor, &WidgetEditor::onEraseCell, this, [&](int x, int y)
        { App::getState()->editorEraseCell(x, y); });

    connect(ui->widgetEditor, &WidgetEditor::onColorPickCell, this, [&](int x, int y)
        { App::getState()->editorColorPickCell(x, y); });

    connect(ui->widgetEditor, &WidgetEditor::onLinkCell, this, [&](int x, int y)
        { App::getState()->editorToggleCellIsLink(x, y); });

    connect(ui->widgetEditor, &WidgetEditor::onHoverCell, this, [&](int x, int y, Cell const * cell)
    {
        if (cell == nullptr)
            ui->lbExtraInfo->setText(QString("X:%1, Y:%2").arg(x).arg(y));
        else
            ui->lbExtraInfo->setText(QString("X:%1, Y:%2, TileID:%3, PaletteID:%4, HFlip:%5, VFlip:%6")
                                 .arg(x)
                                 .arg(y)
                                 .arg(cell->tileID)
                                 .arg(cell->paletteID)
                                 .arg(cell->hFlip)
                                 .arg(cell->vFlip));
    });

    connect(App::getState(), &AppState::onShowLinkInfoChanged, this, [&](bool value)
    {
        ui->widgetEditor->setShowLinkInfo(value);
    });

    connect(ui->widgetEditor, &WidgetEditor::onScrollWheel, this, [&](bool value)
    {
        if (value)
            App::getState()->zoomInEditor();
        else
            App::getState()->zoomOutEditor();
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

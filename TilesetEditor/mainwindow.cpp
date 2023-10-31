#include "app.h"
#include "dialogeditscenes.h"
#include "mainwindow.h"
#include "qlayout.h"
#include "ioservice.h"
#include "ui_mainwindow.h"
#include "dialogoptions.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _fragmentContextOpen(nullptr)
    , _fragmentContextClosed(nullptr)

{
    ui->setupUi(this);

    connect(App::getState(), &AppState::onProjectChanged, this, [&](Project * value) {
        prepareUIForProject(value);
    });

    connect(App::getState(), &AppState::onProjectHasChangesChanged, this, [&](bool value) {
        auto project = App::getState()->project();

        if (project == nullptr)
        {
            setWindowTitle("TilesetEditor");
            return;
        }

        if (value)
            setWindowTitle("*" + project->path);
        else
            setWindowTitle(project->path);
    });

    connect(App::getState(), &AppState::onLastDumpFolderChanged, this, [&](QString const & value) {
        ui->action_File_ReloadDump->setEnabled(!value.isEmpty());
    });

    connect(ui->cbScenes, &QComboBox::currentIndexChanged, this, [&](int index)
    {
        int newSceneID = index < 2 ? index-1 : App::getState()->allScenes()->at(index-2)->id;
        App::getState()->setSelectedSceneID(newSceneID);
    });

    connect(App::getState(), &AppState::onTileFilterChanged, this, [&](TilePreviewFilter const &) { loadFlags(); });
    connect(App::getState(), &AppState::onShowLinkInfoChanged, this, [&](bool) { loadFlags(); });

//    connect(App::getState(), &AppState::onSelectedSceneIDChanged, this, [&](int sceneID)
//    {
//        auto scenes = App::getState()->allScenes();
//        for (int i=0;i!=scenes->size();++i)
//            if (scenes->at(i)->id == sceneID)
//        ui->cbScenes->setCurrentIndex();
//    });

    connect(App::getState(), &AppState::onAllScenesChanged, this, [&](QList<Scene*> const * value)
    {
        saveSelectedScene();
        loadScenes(value);
        restoreSelectedScene();
    });

    // File menu

    connect(ui->action_File_NewProject, &QAction::triggered, this, &MainWindow::onAction_File_NewProject);
    connect(ui->action_File_OpenProject, &QAction::triggered, this, &MainWindow::onAction_File_OpenProject);
    connect(ui->action_File_SaveProject, &QAction::triggered, this, &MainWindow::onAction_File_SaveProject);
    connect(ui->action_File_CloseProject, &QAction::triggered, this, &MainWindow::onAction_File_CloseProject);
    // separator
    connect(ui->action_File_LoadDump, &QAction::triggered, this, &MainWindow::onAction_File_LoadDump);
    connect(ui->action_File_ReloadDump, &QAction::triggered, this, &MainWindow::onAction_File_ReloadDump);
    // separator
    connect(ui->action_File_Quit, &QAction::triggered, this, &MainWindow::onAction_File_QuitProject);

    // Edit menu
    connect(ui->action_Edit_Undo, &QAction::triggered, this, [&]() { App::getState()->editorUndo(); });
    connect(ui->action_Edit_Redo, &QAction::triggered, this, [&]() { App::getState()->editorRedo(); });
    connect(ui->action_Edit_AutoLink, &QAction::triggered, this, [&]() { App::getState()->autoLink(); });
    connect(ui->action_Edit_AutoUnlink, &QAction::triggered, this, [&]() { App::getState()->autoUnlink(); });
    connect(ui->action_Edit_ClearCell, &QAction::triggered, this, [&]() { App::getState()->clearCell(); });

    connect(ui->action_Edit_Scenes, &QAction::triggered, this, &MainWindow::onAction_Edit_Scenes);
    connect(ui->action_Edit_MoveTileToScene, &QAction::triggered, this, &MainWindow::onAction_Edit_MoveTileToScene);
    connect(ui->action_Edit_MoveTilesetToScene, &QAction::triggered, this, &MainWindow::onAction_Edit_MoveTilesetToScene);
    connect(ui->action_Edit_InsertNearestReferenceTile, &QAction::triggered, this, [](){ App::getState()->editorPaintCellUsingSibling(); });
    connect(ui->action_Edit_InsertSelectedTile, &QAction::triggered, this, [](){ App::getState()->editorPaintCellUsingSelection(); });

    connect(ui->action_Edit_MoveCellsInTileset_Down, &QAction::triggered, this, [](){ App::getState()->moveCellsInTileset(0,+1); });
    connect(ui->action_Edit_MoveCellsInTileset_Up, &QAction::triggered, this, [](){ App::getState()->moveCellsInTileset(0,-1); });
    connect(ui->action_Edit_MoveCellsInTileset_Left, &QAction::triggered, this, [](){ App::getState()->moveCellsInTileset(-1,0); });
    connect(ui->action_Edit_MoveCellsInTileset_Right, &QAction::triggered, this, [](){ App::getState()->moveCellsInTileset(+1,0); });

    connect(ui->action_Edit_FlushCellsWithPalette, &QAction::triggered, this, [](){ App::getState()->flushCellsWithSelectedPalettes(); });

    // View menu
    connect(ui->action_View_Reference_1, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_1); });
    connect(ui->action_View_Reference_25, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_25); });
    connect(ui->action_View_Reference_50, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_50); });
    connect(ui->action_View_Reference_75, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_75); });
    connect(ui->action_View_Reference_100, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_100); });

    connect(ui->action_View_Reference_NN, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_NN); });
    connect(ui->action_View_Reference_NF, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_NF); });
    connect(ui->action_View_Reference_FN, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_FN); });
    connect(ui->action_View_Reference_FF, &QAction::triggered, this, [&](){ App::getState()->setReferenceMode(REF_FF); });

    connect(ui->action_View_Reference_ZoomIn, &QAction::triggered, this, [&](){ App::getState()->zoomInReference(); });
    connect(ui->action_View_Reference_ZoomOut, &QAction::triggered, this, [&](){ App::getState()->zoomOutReference(); });

    connect(ui->action_View_Editor_ZoomIn, &QAction::triggered, this, [&](){ App::getState()->zoomInEditor(); });
    connect(ui->action_View_Editor_ZoomOut, &QAction::triggered, this, [&](){ App::getState()->zoomOutEditor(); });

    connect(ui->action_View_FlipTileHorizontally, &QAction::triggered, this, [&]()
    {
        TilePreviewFilter filter = App::getState()->tilePreviewFilter();
        filter.hFlip = !filter.hFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->action_View_FlipTileVertically, &QAction::triggered, this, [&]()
    {
        TilePreviewFilter filter = App::getState()->tilePreviewFilter();
        filter.vFlip = !filter.vFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->action_View_ShowLinkInfo, &QAction::triggered, this, [&]()
    {
        App::getState()->setShowLinkInfo(ui->action_View_ShowLinkInfo->isChecked());
    });

    connect(ui->action_View_FocusEditor, &QAction::triggered, this, [&]() { ui->contentFrame->setFocus(); });

    //Navigate menu
    connect(ui->action_Navigate_Root_Down, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot( 0,+1); });
    connect(ui->action_Navigate_Root_Up, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot( 0,-1); });
    connect(ui->action_Navigate_Root_Left, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot(-1, 0); });
    connect(ui->action_Navigate_Root_Right, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot(+1, 0); });

    connect(ui->action_Navigate_Offset_Down, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset( 0,+1); });
    connect(ui->action_Navigate_Offset_Up, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset( 0,-1); });
    connect(ui->action_Navigate_Offset_Left, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset(-1, 0); });
    connect(ui->action_Navigate_Offset_Right, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset(+1, 0); });

    connect(ui->action_Navigate_Viewport_Down, &QAction::triggered, this, [&](){ App::getState()->moveViewport( 0,+1); });
    connect(ui->action_Navigate_Viewport_Up, &QAction::triggered, this, [&](){ App::getState()->moveViewport( 0,-1); });
    connect(ui->action_Navigate_Viewport_Left, &QAction::triggered, this, [&](){ App::getState()->moveViewport(-1, 0); });
    connect(ui->action_Navigate_Viewport_Right, &QAction::triggered, this, [&](){ App::getState()->moveViewport(+1, 0); });

    connect(ui->action_Navigate_Tile_Down, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection( 0,+1); });
    connect(ui->action_Navigate_Tile_Up, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection( 0,-1); });
    connect(ui->action_Navigate_Tile_Left, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection(-1, 0); });
    connect(ui->action_Navigate_Tile_Right, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection(+1, 0); });

    connect(ui->action_Navigate_Root_Home, &QAction::triggered, this, [&](){ App::getState()->moveEditorRootHome(); });
    connect(ui->action_Navigate_Offset_Home, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffsetHome(); });
    connect(ui->action_Navigate_Viewport_Home, &QAction::triggered, this, [&](){ App::getState()->moveViewportHome(); });

    // Execute menu
    connect(ui->action_Execute_BuildTilesets, &QAction::triggered, this, [&]()
    {
        IOReport report;
        IOService::buildTilesets(&report);
        ui->lbStatusBar->setText(report.message());
    });

    // Help menu

    // Load widgets
    prepareUIForProject(nullptr);

    loadScenes(App::getState()->allScenes());
    ui->cbScenes->setCurrentIndex(1);

    loadFlags();
}

void MainWindow::loadFlags()
{
    auto filter = App::getState()->tilePreviewFilter();

    if (ui->action_View_FlipTileHorizontally->isChecked() != filter.hFlip)
        ui->action_View_FlipTileHorizontally->setChecked(filter.hFlip);

    if (ui->action_View_FlipTileVertically->isChecked() != filter.vFlip)
        ui->action_View_FlipTileVertically->setChecked(filter.vFlip);

    if (ui->action_View_ShowLinkInfo->isChecked() != App::getState()->showLinkInfo())
        ui->action_View_ShowLinkInfo->setChecked(App::getState()->showLinkInfo());
}

void MainWindow::loadScenes(QList<Scene*> const * value)
{
    ui->cbScenes->clear();
    ui->cbScenes->addItem("All Scenes");
    ui->cbScenes->addItem("NULL Scene");

    if (value == nullptr)
        return;

    for (auto s : *value)
    {
        QString number = QString::number(s->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number, s->name);
        ui->cbScenes->addItem(newText);
    }
}

void MainWindow::restoreSelectedScene()
{
    if (_lastSceneID < 1)
    {
        ui->cbScenes->setCurrentIndex(_lastSceneID+1);
        return;
    }

    auto scenes = App::getState()->allScenes();

    for (qsizetype i=0;i!=scenes->size();++i)
    {
        if (scenes->at(i)->id == _lastSceneID)
        {
            ui->cbScenes->setCurrentIndex(i+2);
            return;
        }
    }

    if (!scenes->isEmpty())
        ui->cbScenes->setCurrentIndex(0);
}

void MainWindow::saveSelectedScene()
{
    _lastSceneID = App::getState()->selectedSceneID();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAction_File_NewProject()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList fileNames;

    if (dialog.exec())
    {
        IOReport report;
        fileNames = dialog.selectedFiles();
        IOService::create(fileNames.first(), &report);
        showMessage(report.message());
    }
    else
    {
        qDebug("NewProject canceled by the user");
    }
}

void MainWindow::onAction_File_OpenProject()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    QStringList fileNames;

    if (dialog.exec())
    {
        IOReport report;
        fileNames = dialog.selectedFiles();
        IOService::load(fileNames.first(), &report);
        showMessage(report.message());
    }
    else
    {
        qDebug("OpenProject canceled by the user");
    }
}

void MainWindow::onAction_File_SaveProject()
{
    IOReport report;
    IOService::save(&report);
    showMessage(report.message());
}

void MainWindow::onAction_File_CloseProject()
{
    IOReport report;
    IOService::close(&report);
    showMessage(report.message());
}

void MainWindow::onAction_File_LoadDump()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    QStringList fileNames;

    if (dialog.exec())
    {
        IOReport report;
        fileNames = dialog.selectedFiles();
        IOService::importDump(fileNames.first(), &report);
        App::getState()->setProjectLastDumpFolder(fileNames.first());
        showMessage(report.message());
    }
    else
    {
        qDebug("LoadDump canceled by the user");
    }
}

void MainWindow::onAction_File_ReloadDump()
{
    IOReport report;
    IOService::importDump(App::getState()->projectLastDumpFolder(), &report);
    showMessage(report.message());
}

void MainWindow::onAction_File_QuitProject()
{
    close();
}

void MainWindow::onAction_Edit_Scenes()
{
    DialogEditScenes dialog(this);
    dialog.exec();
    App::getState()->setAllScenes(App::getState()->allScenes());
}

void MainWindow::onAction_Edit_MoveTileToScene()
{
    auto selectedTiles = App::getState()->selectedTiles();

    if (selectedTiles->isEmpty())
        return;

    auto scenes = App::getState()->allScenes();

    QStringList options;
    options.append("NULL Scene");
    for (auto item : *scenes)
        options.append(item->name);

    DialogOptions dialog;
    dialog.setOptions(options, App::getState()->lastMoveToSceneResult());
    dialog.setWindowTitle("Move Tile(s) to Scene");

    if (dialog.exec() && dialog.selectedOption() >= 0 && dialog.selectedOption() <= scenes->size())
    {
        App::getState()->setLastMoveToSceneResult(dialog.selectedOption());

        if (dialog.selectedOption() == 0)
            App::getState()->moveSelectedTilesToScene(0);
        else
            App::getState()->moveSelectedTilesToScene((*scenes)[dialog.selectedOption()-1]->id);
    }
}

void MainWindow::onAction_Edit_MoveTilesetToScene()
{
    if (App::getState()->selectedTileset() == nullptr)
        return;

    auto scenes = App::getState()->allScenes();

    QStringList options;
    options.append("NULL Scene");
    for (auto item : *scenes)
        options.append(item->name);

    DialogOptions dialog;
    dialog.setOptions(options, App::getState()->lastMoveToSceneResult());
    dialog.setWindowTitle("Move Tileset to Scene");

    if (dialog.exec() && dialog.selectedOption() >= 0 && dialog.selectedOption() <= scenes->size())
    {
        App::getState()->setLastMoveToSceneResult(dialog.selectedOption());

        if (dialog.selectedOption() == 0)
            App::getState()->moveSelectedTilesetToScene(0);
        else
            App::getState()->moveSelectedTilesetToScene((*scenes)[dialog.selectedOption()-1]->id);
    }
}

void MainWindow::prepareUIForProject(Project * value)
{
    bool const hasProject = value != nullptr;
    auto layout = ui->contentFrame->layout();

    setWindowTitle(hasProject?value->path:"TilesetEditor");

    if (layout == nullptr)
    {
        layout = new QVBoxLayout();
        layout->addWidget(hasProject?createFragmentContextOpen():createFragmentContextClosed());
        layout->setContentsMargins(0,0,0,0);
        ui->contentFrame->setLayout(layout);
    }
    else
    {
        layout->replaceWidget(layout->itemAt(0)->widget(), hasProject?createFragmentContextOpen():createFragmentContextClosed());
    }

    ui->menu_View_ReferenceScreenshot->setEnabled(hasProject);
    ui->menu_Navigate_Offset->setEnabled(hasProject);
    ui->menu_Navigate_Root->setEnabled(hasProject);
    ui->menu_Navigate_Viewport->setEnabled(hasProject);
    ui->menu_Navigate_Tile->setEnabled(hasProject);
    ui->menu_Edit_MoveTilesInTileset->setEnabled(hasProject);

    ui->menu_Navigate->setEnabled(hasProject);
    ui->menu_View->setEnabled(hasProject);
    ui->menu_Execute->setEnabled(hasProject);
    ui->menu_Edit->setEnabled(hasProject);

    ui->action_File_SaveProject->setEnabled(hasProject);
    ui->action_File_CloseProject->setEnabled(hasProject);
    ui->action_File_LoadDump->setEnabled(hasProject);
    ui->action_File_ReloadDump->setEnabled(hasProject);

    ui->action_Edit_Scenes->setEnabled(hasProject);
    ui->action_Edit_MoveTileToScene->setEnabled(hasProject);
    ui->action_Edit_MoveTilesetToScene->setEnabled(hasProject);
    ui->action_Edit_Undo->setEnabled(hasProject);
    ui->action_Edit_Redo->setEnabled(hasProject);
    ui->action_Edit_AutoLink->setEnabled(hasProject);
    ui->action_Edit_AutoUnlink->setEnabled(hasProject);
    ui->action_Edit_InsertNearestReferenceTile->setEnabled(hasProject);
    ui->action_Edit_InsertSelectedTile->setEnabled(hasProject);
    ui->action_Edit_ClearCell->setEnabled(hasProject);
    ui->action_Edit_MoveCellsInTileset_Down->setEnabled(hasProject);
    ui->action_Edit_MoveCellsInTileset_Up->setEnabled(hasProject);
    ui->action_Edit_MoveCellsInTileset_Left->setEnabled(hasProject);
    ui->action_Edit_MoveCellsInTileset_Right->setEnabled(hasProject);

    ui->action_View_NextTileUsage->setEnabled(hasProject);
    ui->action_View_FlipTileHorizontally->setEnabled(hasProject);
    ui->action_View_FlipTileVertically->setEnabled(hasProject);
    ui->action_View_Reference_ZoomIn->setEnabled(hasProject);
    ui->action_View_Reference_ZoomOut->setEnabled(hasProject);
    ui->action_View_FocusEditor->setEnabled(hasProject);
    ui->action_View_Editor_ZoomIn->setEnabled(hasProject);
    ui->action_View_Editor_ZoomOut->setEnabled(hasProject);
    ui->action_View_Reference_1->setEnabled(hasProject);
    ui->action_View_Reference_25->setEnabled(hasProject);
    ui->action_View_Reference_50->setEnabled(hasProject);
    ui->action_View_Reference_75->setEnabled(hasProject);
    ui->action_View_Reference_100->setEnabled(hasProject);
    ui->action_View_Reference_NN->setEnabled(hasProject);
    ui->action_View_Reference_NF->setEnabled(hasProject);
    ui->action_View_Reference_FN->setEnabled(hasProject);
    ui->action_View_Reference_FF->setEnabled(hasProject);

    ui->action_Navigate_Offset_Down->setEnabled(hasProject);
    ui->action_Navigate_Offset_Up->setEnabled(hasProject);
    ui->action_Navigate_Offset_Left->setEnabled(hasProject);
    ui->action_Navigate_Offset_Right->setEnabled(hasProject);
    ui->action_Navigate_Offset_Home->setEnabled(hasProject);
    ui->action_Navigate_Root_Down->setEnabled(hasProject);
    ui->action_Navigate_Root_Up->setEnabled(hasProject);
    ui->action_Navigate_Root_Left->setEnabled(hasProject);
    ui->action_Navigate_Root_Right->setEnabled(hasProject);
    ui->action_Navigate_Root_Home->setEnabled(hasProject);
    ui->action_Navigate_Tile_Down->setEnabled(hasProject);
    ui->action_Navigate_Tile_Up->setEnabled(hasProject);
    ui->action_Navigate_Tile_Left->setEnabled(hasProject);
    ui->action_Navigate_Tile_Right->setEnabled(hasProject);
    ui->action_Navigate_Viewport_Down->setEnabled(hasProject);
    ui->action_Navigate_Viewport_Up->setEnabled(hasProject);
    ui->action_Navigate_Viewport_Left->setEnabled(hasProject);
    ui->action_Navigate_Viewport_Right->setEnabled(hasProject);
    ui->action_Navigate_Viewport_Home->setEnabled(hasProject);

    ui->action_Execute_BreakTilesets->setEnabled(hasProject);
    ui->action_Execute_BuildTilesets->setEnabled(hasProject);
    ui->action_Execute_EncodeHDTiles->setEnabled(hasProject);
    ui->action_Execute_Pipelines->setEnabled(hasProject);

    ui->action_Help_VerifyInconsistencies->setEnabled(hasProject);

    ui->cbScenes->setVisible(hasProject);
}

QWidget * MainWindow::createFragmentContextOpen()
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);

    _fragmentContextOpen = new FragmentContextOpen(this);
    _fragmentContextOpen->setSizePolicy(policy);

    return _fragmentContextOpen;
}

QWidget * MainWindow::createFragmentContextClosed()
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);

    _fragmentContextClosed = new FragmentContextClosed(this);
    _fragmentContextClosed->setSizePolicy(policy);

    return _fragmentContextClosed;
}

void MainWindow::showMessage(QString msg)
{
    ui->lbStatusBar->setText(msg);
    if (msg != "")
        QTimer::singleShot(5000, this, [&](){ showMessage("") ;});
}


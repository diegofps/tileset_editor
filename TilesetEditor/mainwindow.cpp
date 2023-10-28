#include "app.h"
#include "dialogeditscenes.h"
#include "mainwindow.h"
#include "qlayout.h"
#include "servicecontext.h"
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

    prepareUIForProject(nullptr);

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

    loadScenes(App::getState()->allScenes());
    ui->cbScenes->setCurrentIndex(1);

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
    connect(ui->action_Edit_Scenes, &QAction::triggered, this, &MainWindow::onAction_Edit_Scenes);
    connect(ui->action_Edit_MoveTileToScene, &QAction::triggered, this, &MainWindow::onAction_Edit_MoveTileToScene);
    connect(ui->action_Edit_MoveTilesetToScene, &QAction::triggered, this, &MainWindow::onAction_Edit_MoveTilesetToScene);
    connect(ui->action_Edit_InsertNearestReferenceTile, &QAction::triggered, this, [](){ App::getState()->editorPaintCellUsingSibling(); });


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
        TileFilter filter = App::getState()->tileMode();
        filter.hFlip = !filter.hFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->action_View_FlipTileVertically, &QAction::triggered, this, [&]()
    {
        TileFilter filter = App::getState()->tileMode();
        filter.vFlip = !filter.vFlip;
        App::getState()->setTilePreviewFilter(filter);
    });

    connect(ui->action_View_FocusEditor, &QAction::triggered, this, [&]() { ui->contentFrame->setFocus(); });

    //Navigate menu
    connect(ui->action_Navigate_Editor_Down, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot( 0,+1); });
    connect(ui->action_Navigate_Editor_Up, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot( 0,-1); });
    connect(ui->action_Navigate_Editor_Left, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot(-1, 0); });
    connect(ui->action_Navigate_Editor_Right, &QAction::triggered, this, [&](){ App::getState()->moveEditorRoot(+1, 0); });

    connect(ui->action_Navigate_References_Down, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset( 0,+1); });
    connect(ui->action_Navigate_References_Up, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset( 0,-1); });
    connect(ui->action_Navigate_References_Left, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset(-1, 0); });
    connect(ui->action_Navigate_References_Right, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffset(+1, 0); });

    connect(ui->action_Navigate_Viewport_Down, &QAction::triggered, this, [&](){ App::getState()->moveViewport( 0,+1); });
    connect(ui->action_Navigate_Viewport_Up, &QAction::triggered, this, [&](){ App::getState()->moveViewport( 0,-1); });
    connect(ui->action_Navigate_Viewport_Left, &QAction::triggered, this, [&](){ App::getState()->moveViewport(-1, 0); });
    connect(ui->action_Navigate_Viewport_Right, &QAction::triggered, this, [&](){ App::getState()->moveViewport(+1, 0); });

    connect(ui->action_Navigate_Tile_Down, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection( 0,+1); });
    connect(ui->action_Navigate_Tile_Up, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection( 0,-1); });
    connect(ui->action_Navigate_Tile_Left, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection(-1, 0); });
    connect(ui->action_Navigate_Tile_Right, &QAction::triggered, this, [&](){ App::getState()->moveTileSelection(+1, 0); });

    connect(ui->action_Navigate_Editor_Home, &QAction::triggered, this, [&](){ App::getState()->moveEditorRootHome(); });
    connect(ui->action_Navigate_References_Home, &QAction::triggered, this, [&](){ App::getState()->moveReferenceOffsetHome(); });
    connect(ui->action_Navigate_Viewport_Home, &QAction::triggered, this, [&](){ App::getState()->moveViewportHome(); });

    // Execute menu

    // Help menu
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
        ContextReport report;
        fileNames = dialog.selectedFiles();
        ServiceContext::create(fileNames.first(), &report);
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
        ContextReport report;
        fileNames = dialog.selectedFiles();
        ServiceContext::load(fileNames.first(), &report);
        showMessage(report.message());
    }
    else
    {
        qDebug("OpenProject canceled by the user");
    }
}

void MainWindow::onAction_File_SaveProject()
{
    ContextReport report;
    ServiceContext::save(&report);
    showMessage(report.message());
}

void MainWindow::onAction_File_CloseProject()
{
    ContextReport report;
    ServiceContext::close(&report);
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
        ContextReport report;
        fileNames = dialog.selectedFiles();
        ServiceContext::importDump(fileNames.first(), &report);
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
    ContextReport report;
    ServiceContext::importDump(App::getState()->projectLastDumpFolder(), &report);
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

    ui->action_File_SaveProject->setEnabled(hasProject);
    ui->action_File_CloseProject->setEnabled(hasProject);
    ui->action_File_LoadDump->setEnabled(hasProject);
    ui->action_File_ReloadDump->setEnabled(hasProject);

    ui->action_Execute_BreakTilesets->setEnabled(hasProject);
    ui->action_Execute_BuildTilesets->setEnabled(hasProject);
    ui->action_Execute_EncodeHDTiles->setEnabled(hasProject);
    ui->action_Execute_Pipelines->setEnabled(hasProject);

    ui->action_View_NextTileUsage->setEnabled(hasProject);
    ui->action_View_Reference_1->setEnabled(hasProject);
    ui->action_View_Reference_25->setEnabled(hasProject);
    ui->action_View_Reference_50->setEnabled(hasProject);
    ui->action_View_Reference_75->setEnabled(hasProject);
    ui->action_View_Reference_100->setEnabled(hasProject);
    ui->action_View_Reference_NN->setEnabled(hasProject);
    ui->action_View_Reference_NF->setEnabled(hasProject);
    ui->action_View_Reference_FN->setEnabled(hasProject);
    ui->action_View_Reference_FF->setEnabled(hasProject);

    ui->action_Edit_Scenes->setEnabled(hasProject);
    ui->action_Edit_MoveTileToScene->setEnabled(hasProject);
    ui->action_Edit_MoveTilesetToScene->setEnabled(hasProject);

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


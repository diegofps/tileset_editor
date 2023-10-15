#include "app.h"
#include "mainwindow.h"
#include "qlayout.h"
#include "servicecontext.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _fragmentContextOpen(nullptr)
    , _fragmentContextClosed(nullptr)

{
    ui->setupUi(this);

    prepareUIForContext("");

    connect(App::getState(), &AppState::onContextFolderChanged, this, [&](QString const & value) {
        prepareUIForContext(value);
    });

    connect(App::getState(), &AppState::onContextLastDumpFolderChanged, this, [&](QString const & value) {
        ui->action_File_ReloadDump->setEnabled(!value.isEmpty());
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

    // View menu

    // Execute menu

    // Help menu
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

    QStringList fileNames;

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        ServiceContext::create(fileNames.first());
//        App::getState()->setContextFolder(fileNames.first());
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
        fileNames = dialog.selectedFiles();
        ServiceContext::load(fileNames.first());
//        App::getState()->setContextFolder(fileNames.first());
    }
    else
    {
        qDebug("OpenProject canceled by the user");
    }
}

void MainWindow::onAction_File_SaveProject()
{
    ServiceContext::save();
}

void MainWindow::onAction_File_CloseProject()
{
//    App::getState()->setContextFolder("");
    ServiceContext::close();
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
        fileNames = dialog.selectedFiles();
        ServiceContext::loadDump(fileNames.first());
        App::getState()->setContextLastDumpFolder(fileNames.first());
//        App::getState()->setContextFolder(fileNames.first());
    }
    else
    {
        qDebug("LoadDump canceled by the user");
    }
}

void MainWindow::onAction_File_ReloadDump()
{
    ServiceContext::loadDump(App::getState()->contextLastDumpFolder());
}

void MainWindow::onAction_File_QuitProject()
{
    close();
}

void MainWindow::prepareUIForContext(QString value)
{
    bool const hasContext = !value.isEmpty();

    if (hasContext)
    {
        setWindowTitle(value);
        setCentralWidget(createFragmentContextOpen());
    }
    else
    {
        setWindowTitle("TilesetEditor");
        setCentralWidget(createFragmentContextClosed());
    }

    ui->action_File_SaveProject->setEnabled(hasContext);
    ui->action_File_CloseProject->setEnabled(hasContext);
    ui->action_File_LoadDump->setEnabled(hasContext);
    ui->action_File_ReloadDump->setEnabled(hasContext);

    ui->action_Execute_BreakTilesets->setEnabled(hasContext);
    ui->action_Execute_BuildTilesets->setEnabled(hasContext);
    ui->action_Execute_EncodeHDTiles->setEnabled(hasContext);
    ui->action_Execute_Pipelines->setEnabled(hasContext);

    ui->action_View_References->setEnabled(hasContext);
    ui->action_View_Editor->setEnabled(hasContext);
    ui->action_View_ResetLayout->setEnabled(hasContext);
}

FragmentContextOpen * MainWindow::createFragmentContextOpen()
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

FragmentContextClosed * MainWindow::createFragmentContextClosed()
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


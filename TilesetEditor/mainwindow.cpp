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

    connect(App::getState(), &AppState::onProjectFolderChanged, this, [&](QString const & value) {
        prepareUIForContext(value);
    });

    connect(App::getState(), &AppState::onProjectLastDumpFolderChanged, this, [&](QString const & value) {
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
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList fileNames;

    if (dialog.exec())
    {
        ContextReport report;
        fileNames = dialog.selectedFiles();
        ServiceContext::create(fileNames.first(), &report);
        ui->statusBar->showMessage(report.message());
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
        ContextReport report;
        fileNames = dialog.selectedFiles();
        ServiceContext::load(fileNames.first(), &report);
        ui->statusBar->showMessage(report.message());
//        App::getState()->setContextFolder(fileNames.first());
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
    ui->statusBar->showMessage(report.message());
}

void MainWindow::onAction_File_CloseProject()
{
//    App::getState()->setContextFolder("");
    ContextReport report;
    ServiceContext::close(&report);
    ui->statusBar->showMessage(report.message());
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
        ui->statusBar->showMessage(report.message());
//        App::getState()->setContextFolder(fileNames.first());
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
    ui->statusBar->showMessage(report.message());
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


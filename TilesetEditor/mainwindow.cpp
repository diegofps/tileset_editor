#include "app.h"
#include "mainwindow.h"
#include "qlayout.h"
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

    connect(App::getState(), &AppState::onContextFolderChanged, this, [&](QString const & value) { prepareUIForContext(value); });

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onActionQuit);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onActionClose);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareUIForContext(QString value)
{
    if (value.isEmpty())
    {
        setWindowTitle("TilesetEditor");
        setCentralWidget(getFragmentContextClosed());
        ui->actionSave->setEnabled(false);
        ui->actionClose->setEnabled(false);
        ui->actionBreak_Tilesets->setEnabled(false);
        ui->actionBuild_tilesets->setEnabled(false);
        ui->actionEncode_HD_tiles->setEnabled(false);
        ui->actionPipelines->setEnabled(false);
        ui->actionReload_dump->setEnabled(false);
        ui->actionReferences->setEnabled(false);
        ui->actionEditor->setEnabled(false);
        ui->actionReset_Layout->setEnabled(false);
    }
    else
    {
        setWindowTitle(value);
        setCentralWidget(getFragmentContextOpen());
        ui->actionSave->setEnabled(true);
        ui->actionClose->setEnabled(true);
        ui->actionBreak_Tilesets->setEnabled(true);
        ui->actionBuild_tilesets->setEnabled(true);
        ui->actionEncode_HD_tiles->setEnabled(true);
        ui->actionPipelines->setEnabled(true);
        ui->actionReload_dump->setEnabled(true);
        ui->actionReferences->setEnabled(true);
        ui->actionEditor->setEnabled(true);
        ui->actionReset_Layout->setEnabled(true);
    }
}

FragmentContextOpen * MainWindow::getFragmentContextOpen()
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

FragmentContextClosed * MainWindow::getFragmentContextClosed()
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

void MainWindow::onActionOpen()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    QStringList fileNames;

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        App::getState()->setContextFolder(fileNames.first());
    }
    else
    {
        qDebug("ActionOpen canceled by the user");
    }
}


void MainWindow::onActionQuit()
{
    close();
}

void MainWindow::onActionClose()
{
    App::getState()->setContextFolder("");
}


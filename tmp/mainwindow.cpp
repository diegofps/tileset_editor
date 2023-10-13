#include "app.h"
#include "mainwindow.h"
#include "qlayout.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _fragmentContextOpen(nullptr)
    , _fragmentContextClosed(nullptr)

{
    ui->setupUi(this);

    prepareUIForContext("");

    connect(App::getState(), &AppState::onContextFolderChanged, this, [&](QString const & value) { prepareUIForContext(value); });

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onActionQuit);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareUIForContext(QString value)
{
    if (value.isEmpty())
    {
        setCentralWidget(getFragmentContextClosed());
        ui->actionSave->setEnabled(false);
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
        setCentralWidget(getFragmentContextOpen());
        ui->actionSave->setEnabled(true);
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
    QWidget * fragment = layout()->itemAt(0)->widget();

    if (fragment->inherits(FragmentContextOpen::staticMetaObject.className()))
        App::getState()->setContextFolder("");
    else if (fragment->inherits(FragmentContextClosed::staticMetaObject.className()))
        App::getState()->setContextFolder("/long/path/to/rom.context");
    else
        App::getState()->setContextFolder("/unknown_path");
}


void MainWindow::onActionQuit()
{
    close();
}


void MainWindow::onActionViewEditor()
{
    App::getState()->setPreviewPage("editor");
}

void MainWindow::onActionViewReferences()
{
    App::getState()->setPreviewPage("references");
}

void MainWindow::onActionViewResetLayout()
{

}

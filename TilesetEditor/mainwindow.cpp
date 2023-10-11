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

    setCentralWidget(getFragmentContextClosed());

    connect(&App::getState(), &AppState::onContextFolderChanged, this, [&](QString & value)
    {
        if (value.isEmpty())
            setCentralWidget(getFragmentContextClosed());
        else
            setCentralWidget(getFragmentContextOpen());

        qDebug("Layout has %d items", layout()->count());
    });

    qDebug("Layout has %d items", layout()->count());
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_actionOpen_triggered()
{
    QWidget * fragment = layout()->itemAt(0)->widget();

    if (fragment->inherits(FragmentContextOpen::staticMetaObject.className()))
        App::getState().setContextFolder("");
    else if (fragment->inherits(FragmentContextClosed::staticMetaObject.className()))
        App::getState().setContextFolder("/long/path/to/rom.context");
    else
        App::getState().setContextFolder("/unknown_path");
}


void MainWindow::on_actionQuit_triggered()
{
    close();
}


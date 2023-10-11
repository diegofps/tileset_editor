#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fragmentcontextclosed.h"
#include "fragmentcontextopen.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

private:

    FragmentContextOpen * getFragmentContextOpen();

    FragmentContextClosed * getFragmentContextClosed();


private:
    Ui::MainWindow *ui;
    FragmentContextOpen * _fragmentContextOpen;
    FragmentContextClosed * _fragmentContextClosed;
};
#endif // MAINWINDOW_H

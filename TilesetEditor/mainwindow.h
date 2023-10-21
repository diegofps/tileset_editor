#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fragmentcontextclosed.h"
#include "fragmentcontextopen.h"
#include "model/project.h"

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
    void onAction_File_NewProject();
    void onAction_File_OpenProject();
    void onAction_File_SaveProject();
    void onAction_File_CloseProject();
    void onAction_File_LoadDump();
    void onAction_File_ReloadDump();
    void onAction_File_QuitProject();

    void onAction_View_References();
    void onAction_Edit_Clusters();

private:
    void prepareUIForProject(Project * value);
    FragmentContextOpen * createFragmentContextOpen();
    FragmentContextClosed * createFragmentContextClosed();

private:
    Ui::MainWindow *ui;
    FragmentContextOpen * _fragmentContextOpen;
    FragmentContextClosed * _fragmentContextClosed;

};
#endif // MAINWINDOW_H

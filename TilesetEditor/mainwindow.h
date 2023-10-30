#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fragmentcontextclosed.h"
#include "fragmentcontextopen.h"
#include "model/project.h"
#include "model/scene.h"

#include <QMainWindow>
#include <QTimer>

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

    void onAction_Edit_Scenes();
    void onAction_Edit_MoveTileToScene();
    void onAction_Edit_MoveTilesetToScene();

//    void onAction_View_Reference(int position);

private:
    void prepareUIForProject(Project * value);
    QWidget * createFragmentContextOpen();
    QWidget * createFragmentContextClosed();
    void showMessage(QString msg);
    void loadScenes(QList<Scene*> const * value);
    void restoreSelectedScene();
    void saveSelectedScene();
    void loadFlags();

private:
    Ui::MainWindow * ui;
    FragmentContextOpen * _fragmentContextOpen;
    FragmentContextClosed * _fragmentContextClosed;
    int _lastSceneID;

};
#endif // MAINWINDOW_H

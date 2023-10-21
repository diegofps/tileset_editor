#ifndef DIALOGEDITSCENES_H
#define DIALOGEDITSCENES_H

#include "model/scene.h"
#include <QDialog>

namespace Ui {
class DialogEditScenes;
}

class DialogEditScenes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditScenes(QWidget *parent = nullptr);
    ~DialogEditScenes();

    bool eventFilter(QObject *watched, QEvent *event);

private:
    void loadClusters(const QList<Scene *> *value);
    void onSelectedClusterChanged();

private slots:
    void onBtNewClicked();
    void onBtRemoveClicked();
    void onBtMoveUpClicked();
    void onBtMoveDownClicked();

private:
    Ui::DialogEditScenes *ui;
    Scene * _selectedScene;

};

#endif // DIALOGEDITSCENES_H

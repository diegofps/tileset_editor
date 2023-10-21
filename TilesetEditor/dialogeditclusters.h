#ifndef DIALOGEDITCLUSTERS_H
#define DIALOGEDITCLUSTERS_H

#include "model/cluster.h"
#include <QDialog>

namespace Ui {
class DialogEditClusters;
}

class DialogEditClusters : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditClusters(QWidget *parent = nullptr);
    ~DialogEditClusters();

    bool eventFilter(QObject *watched, QEvent *event);

private:
    void loadClusters(const QList<Cluster *> *value);
    void onSelectedClusterChanged();

private slots:
    void onBtNewClicked();
    void onBtRemoveClicked();
    void onBtMoveUpClicked();
    void onBtMoveDownClicked();

private:
    Ui::DialogEditClusters *ui;
    Cluster * _selectedCluster;

};

#endif // DIALOGEDITCLUSTERS_H

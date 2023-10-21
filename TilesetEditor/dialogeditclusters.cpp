#include "app.h"
#include "dialogeditclusters.h"
#include "ui_dialogeditclusters.h"

#include <QKeyEvent>

DialogEditClusters::DialogEditClusters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditClusters)
{
    ui->setupUi(this);

    ui->btNew->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btRemove->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveUp->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveDown->setStyleSheet(App::getStyles()->get("button_click"));

    connect(App::getState(), &AppState::onProjectClustersChanged, this, [&](QList<Cluster*> const * value)
    {
        loadClusters(value);
        if (value != nullptr && ui->listClusters->currentRow() < 0 && !value->isEmpty())
            ui->listClusters->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onProjectClustersInserted, this, [&](QList<Cluster*> const * value, int const position)
    {
        loadClusters(value);
        ui->listClusters->setCurrentRow(position);
    });

    connect(App::getState(), &AppState::onProjectClustersRemoved, this, [&](QList<Cluster*> const * value, int const position)
    {
        loadClusters(value);
        if (value->size() != 0)
            ui->listClusters->setCurrentRow(position >= value->size() ? value->size()-1 : position);
    });

    connect(App::getState(), &AppState::onProjectClustersMoved, this, [&](QList<Cluster*> const * value, int const oldPosition, int const newPosition)
    {
        (void)oldPosition;
        loadClusters(value);
        ui->listClusters->setCurrentRow(newPosition);
    });

    connect(ui->btNew, &QPushButton::clicked, this, &DialogEditClusters::onBtNewClicked);
    connect(ui->btRemove, &QPushButton::clicked, this, &DialogEditClusters::onBtRemoveClicked);
    connect(ui->btMoveUp, &QPushButton::clicked, this, &DialogEditClusters::onBtMoveUpClicked);
    connect(ui->btMoveDown, &QPushButton::clicked, this, &DialogEditClusters::onBtMoveDownClicked);

    connect(ui->listClusters, &QListWidget::currentRowChanged, this, [&](int position)
    {
        auto clusters = App::getState()->projectClusters();

        if (clusters != nullptr && position >= 0 && position < clusters->size())
        {
            _selectedCluster = (*clusters)[position];
            onSelectedClusterChanged();
        }
    });

    connect(ui->leName, &QLineEdit::textChanged, this, [&](QString const & text)
    {
        if (_selectedCluster != nullptr && _selectedCluster->name != text)
        {
            _selectedCluster->name = text;
            App::getState()->setProjectHasChanges(true);

            if (ui->listClusters->currentRow() < 0)
                return;

            auto item = ui->listClusters->item(ui->listClusters->currentRow());

            if (item->text() != text)
                item->setText(text);
        }
    });

    connect(ui->leName, &QLineEdit::returnPressed, this, [&]()
    {
        qDebug() << "returnPressed";
    });

    auto clusters = App::getState()->projectClusters();
    loadClusters(clusters);
    if (clusters->size() == 0)
    {
        _selectedCluster = nullptr;
    }
    else
    {
        _selectedCluster = (*clusters)[0];
        ui->listClusters->setCurrentRow(0);
    }

    ui->leName->installEventFilter(this);
    ui->leName->setFocus();

    onSelectedClusterChanged();

}

DialogEditClusters::~DialogEditClusters()
{
    delete ui;
}

void DialogEditClusters::onBtNewClicked()
{
    qDebug() << "btNew";
    auto cluster = new Cluster();
    cluster->id = ++App::getState()->project()->lastPaletteID;
    cluster->name = "Noname";

    int const position = ui->listClusters->currentRow() < 0 ? 0 : ui->listClusters->currentRow()+1;
    App::getState()->insertProjectCluster(position, cluster);
    App::getState()->setProjectHasChanges(true);
}

void DialogEditClusters::onBtRemoveClicked()
{
    if (ui->listClusters->currentRow() >= 0)
    {
        App::getState()->removeProjectCluster(ui->listClusters->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

void DialogEditClusters::onBtMoveUpClicked()
{
    if (ui->listClusters->currentRow() >= 0)
    {
        App::getState()->moveDownProjectCluster(ui->listClusters->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

void DialogEditClusters::onBtMoveDownClicked()
{
    if (ui->listClusters->currentRow() >= 0)
    {
        App::getState()->moveUpProjectCluster(ui->listClusters->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

bool DialogEditClusters::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->leName && event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);

        bool up = false;
        bool down = false;

        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
        {
            if (keyEvent->key() == Qt::Key_Return)
                up = true;
        }
        else
        {
            if (keyEvent->key() == Qt::Key_Return)
                down = true;
            if (keyEvent->key() == Qt::Key_Down)
                down = true;
            if (keyEvent->key() == Qt::Key_Up)
                up = true;
        }

        if (down)
        {
            if (ui->listClusters->currentRow() >= ui->listClusters->count()-1)
                ui->listClusters->setCurrentRow(0);
            else
                ui->listClusters->setCurrentRow(ui->listClusters->currentRow()+1);
            ui->leName->selectAll();
            return true;
        }

        if (up)
        {
            if (ui->listClusters->currentRow() <= 0)
                ui->listClusters->setCurrentRow(ui->listClusters->count()-1);
            else
                ui->listClusters->setCurrentRow(ui->listClusters->currentRow()-1);
            ui->leName->selectAll();
            return true;
        }
    }

    return false;
}

void DialogEditClusters::onSelectedClusterChanged()
{
    if (_selectedCluster == nullptr)
    {
        ui->leName->setEnabled(false);
        ui->leName->setText("");
    }
    else
    {
        ui->leName->setEnabled(true);
        ui->leName->setText(_selectedCluster->name);
    }
}

void DialogEditClusters::loadClusters(QList<Cluster*> const * value)
{
    ui->listClusters->clear();

    if (value == nullptr)
        return;

    for (auto value : *value)
        ui->listClusters->addItem(value->name);
}


#include "app.h"
#include "dialogeditscenes.h"
#include "ui_dialogeditscenes.h"

#include <QKeyEvent>

DialogEditScenes::DialogEditScenes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditScenes)
{
    ui->setupUi(this);

    ui->btNew->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btRemove->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveUp->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btMoveDown->setStyleSheet(App::getStyles()->get("button_click"));

    connect(App::getState(), &AppState::onAllScenesChanged, this, [&](QList<Scene*> const * value)
    {
        loadClusters(value);
        if (value != nullptr && ui->listScenes->currentRow() < 0 && !value->isEmpty())
            ui->listScenes->setCurrentRow(0);
    });

    connect(App::getState(), &AppState::onSceneInserted, this, [&](QList<Scene*> const * value, int const position)
    {
        loadClusters(value);
        ui->listScenes->setCurrentRow(position);
    });

    connect(App::getState(), &AppState::onSceneRemoved, this, [&](QList<Scene*> const * value, int const position)
    {
        loadClusters(value);
        if (value->size() != 0)
            ui->listScenes->setCurrentRow(position >= value->size() ? value->size()-1 : position);
    });

    connect(App::getState(), &AppState::onSceneMoved, this, [&](QList<Scene*> const * value, int const oldPosition, int const newPosition)
    {
        (void) oldPosition;
        loadClusters(value);
        ui->listScenes->setCurrentRow(newPosition);
    });

    connect(ui->btNew, &QPushButton::clicked, this, &DialogEditScenes::onBtNewClicked);
    connect(ui->btRemove, &QPushButton::clicked, this, &DialogEditScenes::onBtRemoveClicked);
    connect(ui->btMoveUp, &QPushButton::clicked, this, &DialogEditScenes::onBtMoveUpClicked);
    connect(ui->btMoveDown, &QPushButton::clicked, this, &DialogEditScenes::onBtMoveDownClicked);

    connect(ui->listScenes, &QListWidget::currentRowChanged, this, [&](int position)
    {
        auto scenes = App::getState()->allScenes();

        if (scenes != nullptr && position >= 0 && position < scenes->size())
        {
            _selectedScene = (*scenes)[position];
            onSelectedClusterChanged();
        }
    });

    connect(ui->leName, &QLineEdit::textChanged, this, [&](QString const & text)
    {
        if (_selectedScene != nullptr && _selectedScene->name != text)
        {
            _selectedScene->name = text;
            App::getState()->setProjectHasChanges(true);

            if (ui->listScenes->currentRow() < 0)
                return;

            QString number = QString::number(_selectedScene->id).rightJustified(3, '0');
            QString newText = QString("%1: %2").arg(number, _selectedScene->name);

            auto item = ui->listScenes->item(ui->listScenes->currentRow());

            if (item->text() != newText)
                item->setText(newText);
        }
    });

    connect(ui->leName, &QLineEdit::returnPressed, this, [&]()
    {
        qDebug() << "returnPressed";
    });

    auto scenes = App::getState()->allScenes();
    loadClusters(scenes);
    if (scenes->size() == 0)
    {
        _selectedScene = nullptr;
    }
    else
    {
        _selectedScene = (*scenes)[0];
        ui->listScenes->setCurrentRow(0);
    }

    ui->leName->installEventFilter(this);
    ui->leName->setFocus();

    onSelectedClusterChanged();

}

DialogEditScenes::~DialogEditScenes()
{
    delete ui;
}

void DialogEditScenes::onBtNewClicked()
{
    qDebug() << "btNew";
    auto scene = new Scene();
    scene->id = ++App::getState()->project()->lastSceneID;
    scene->name = "Noname";

    int const position = ui->listScenes->currentRow() < 0 ? 0 : ui->listScenes->currentRow()+1;
    App::getState()->insertScene(position, scene);
    App::getState()->setProjectHasChanges(true);
}

void DialogEditScenes::onBtRemoveClicked()
{
    if (ui->listScenes->currentRow() >= 0)
    {
        App::getState()->removeScene(ui->listScenes->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

void DialogEditScenes::onBtMoveUpClicked()
{
    if (ui->listScenes->currentRow() >= 0)
    {
        App::getState()->moveDownScene(ui->listScenes->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

void DialogEditScenes::onBtMoveDownClicked()
{
    if (ui->listScenes->currentRow() >= 0)
    {
        App::getState()->moveUpScene(ui->listScenes->currentRow());
        App::getState()->setProjectHasChanges(true);
    }
}

bool DialogEditScenes::eventFilter(QObject *watched, QEvent *event)
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
            if (ui->listScenes->currentRow() >= ui->listScenes->count()-1)
                ui->listScenes->setCurrentRow(0);
            else
                ui->listScenes->setCurrentRow(ui->listScenes->currentRow()+1);
            ui->leName->selectAll();
            return true;
        }

        if (up)
        {
            if (ui->listScenes->currentRow() <= 0)
                ui->listScenes->setCurrentRow(ui->listScenes->count()-1);
            else
                ui->listScenes->setCurrentRow(ui->listScenes->currentRow()-1);
            ui->leName->selectAll();
            return true;
        }
    }

    return false;
}

void DialogEditScenes::onSelectedClusterChanged()
{
    if (_selectedScene == nullptr)
    {
        ui->leName->setEnabled(false);
        ui->leName->setText("");
    }
    else
    {
        ui->leName->setEnabled(true);
        ui->leName->setText(_selectedScene->name);
    }
}

void DialogEditScenes::loadClusters(QList<Scene*> const * value)
{
    ui->listScenes->clear();

    if (value == nullptr)
        return;

    for (auto value : *value)
    {
        QString number = QString::number(value->id).rightJustified(3, '0');
        QString newText = QString("%1: %2").arg(number, value->name);
        ui->listScenes->addItem(newText);
    }
}


#include "app.h"
#include "dialogoptions.h"

#include <QKeyEvent>

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    ui->btMove->setStyleSheet(App::getStyles()->get("button_click"));
    ui->btCancel->setStyleSheet(App::getStyles()->get("button_click"));

    connect(ui->btMove, &QPushButton::clicked, this, [&](){ accept(); });
    connect(ui->btCancel, &QPushButton::clicked, this, [&](){ reject(); });

    ui->listWidget->setFocus();
    ui->listWidget->installEventFilter(this);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::setOptions(QStringList options, int defaultSelection)
{
    ui->listWidget->clear();

    if (options.isEmpty())
        return;

    for (QString & item : options)
        ui->listWidget->addItem(item);

    if (defaultSelection >= 0 && defaultSelection < options.size())
        ui->listWidget->setCurrentRow(defaultSelection);
    else
        ui->listWidget->setCurrentRow(0);
}

int DialogOptions::selectedOption() const
{
    return ui->listWidget->currentRow();
}

bool DialogOptions::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->listWidget && event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return)
        {
            accept();
            return true;
        }
    }

    return false;
}

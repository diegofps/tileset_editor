#include "fragmentreferences.h"
#include "ui_fragmentreferences.h"
#include "app.h"

#include <QFile>
#include <QDir>


void FragmentReferences::styleScreenshotButtons(ReferenceMode const value)
{
    auto checked = App::getStyles()->get("button_checked");
    auto unchecked = App::getStyles()->get("button_unchecked");

    ui->btRef1->setStyleSheet(value==REF_1?checked:unchecked);
    ui->btRef25->setStyleSheet(value==REF_25?checked:unchecked);
    ui->btRef50->setStyleSheet(value==REF_50?checked:unchecked);
    ui->btRef75->setStyleSheet(value==REF_75?checked:unchecked);
    ui->btRef100->setStyleSheet(value==REF_100?checked:unchecked);

    ui->btRefNN->setStyleSheet(value==REF_NN?checked:unchecked);
    ui->btRefNF->setStyleSheet(value==REF_NF?checked:unchecked);
    ui->btRefFN->setStyleSheet(value==REF_FN?checked:unchecked);
    ui->btRefFF->setStyleSheet(value==REF_FF?checked:unchecked);

    ui->btRef1->update();
    ui->btRef25->update();
    ui->btRef50->update();
    ui->btRef75->update();
    ui->btRef100->update();

    ui->btRefNN->update();
    ui->btRefNF->update();
    ui->btRefFN->update();
    ui->btRefFF->update();
}

FragmentReferences::FragmentReferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentReferences)
{
    ui->setupUi(this);

    styleScreenshotButtons(App::getState()->referenceMode());

    connect(ui->btRef1, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_1); });
    connect(ui->btRef25, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_25); });
    connect(ui->btRef50, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_50); });
    connect(ui->btRef75, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_75); });
    connect(ui->btRef100, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_100); });

    connect(ui->btRefNN, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_NN); });
    connect(ui->btRefNF, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_NF); });
    connect(ui->btRefFN, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_FN); });
    connect(ui->btRefFF, &QPushButton::clicked, this, [&](){ App::getState()->setReferenceMode(REF_FF); });

    connect(App::getState(), &AppState::onReferenceModeChanged, this, [&](ReferenceMode const value)
    {
        styleScreenshotButtons(value);
        updateReferenceWidget(App::getState()->selectedTiles(), value);
    });

    connect(App::getState(), &AppState::onSelectedTilesChanged, this, [&](QList<Tile*> const * value)
    {
        updateReferenceWidget(value, App::getState()->referenceMode());
    });

    connect(App::getState(), &AppState::onReferenceOffsetChanged, this, [&](QPoint const value)
    {
        ui->widgetReference->setOffset(value.x()*8, value.y()*8);
        QImage * img = ui->widgetReference->offsetImage();
        App::getState()->setReferenceOffsetImage(img);
    });

    connect(App::getState(), &AppState::onReferenceZoomChanged, this, [&](int value)
    {
        ui->widgetReference->setZoom(value);
    });

    ui->widgetReference->setZoom(App::getState()->referenceZoom());
    updateReferenceWidget(App::getState()->selectedTiles(), App::getState()->referenceMode());
}

FragmentReferences::~FragmentReferences()
{
    delete ui;
}

inline QImage * loadScreenshot(Reference const * const reference)
{
    if (reference == nullptr)
        return nullptr;

    auto project = App::getState()->project();

    if (project == nullptr)
        return nullptr;

    QDir dir(project->path);

    if (dir.cd("screenshots"))
    {
        QString basename = QString::number(reference->screenshotId).rightJustified(6, '0');
        QString filename = QString("%1.png").arg(basename);
        QFile file = dir.filePath(filename);

        file.open(QFile::ReadOnly);

        if (file.isOpen())
            return new QImage(QImage::fromData(file.readAll()));
    }
    else
    {
        qWarning() << "Missing screenshots directory in project path";
        return nullptr;
    }

    for (auto s : *App::getState()->projectScreenshots())
        if (s->id == reference->screenshotId)
            return new QImage(QImage::fromData(s->data));

    qWarning() << "Could not load screenshot from neither filepath nor unsaved memory";

    return nullptr;
}

void FragmentReferences::updateReferenceWidget(QList<Tile *> const * tiles, ReferenceMode const value)
{
    ui->widgetReference->setImage(nullptr);
    ui->lbExtraInfo->setText("");

    if (tiles == nullptr || tiles->isEmpty() || tiles->at(0) == nullptr)
        return;

    Tile * tile = tiles->at(0);
    int referenceID;

    switch (value)
    {
    case REF_1: referenceID = tile->ref1ID; break;
    case REF_25: referenceID = tile->ref25ID; break;
    case REF_50: referenceID = tile->ref50ID; break;
    case REF_75: referenceID = tile->ref75ID; break;
    case REF_100: referenceID = tile->ref100ID; break;
    case REF_NN: referenceID = tile->refNNID; break;
    case REF_NF: referenceID = tile->refNFID; break;
    case REF_FN: referenceID = tile->refFNID; break;
    case REF_FF: referenceID = tile->refFFID; break;
    default: referenceID = 0;
    }

    Reference const * const reference = App::getState()->getProjectReferenceById(referenceID);

    if (reference == nullptr)
    {
        qWarning() << "Could not find reference referred by tile, referenceID: " << referenceID;
        return;
    }

    ui->widgetReference->setRoot(reference->x, reference->y);
    ui->widgetReference->setImage(loadScreenshot(reference));

    ui->lbExtraInfo->setText(QString("TileID:%1, PaletteID:%2, HFlip:%3, VFlip:%4, LineCout:%5, MATH:%6, PIXEL:%7, OP:%8, BPSTART:%9, TILE:%10")
                             .arg(tile->id)
                             .arg(reference->colorPaletteID)
                             .arg(reference->hFlip)
                             .arg(reference->vFlip)
                             .arg(reference->lineCount)
                             .arg(reference->MATH)
                             .arg(reference->PIXEL)
                             .arg(reference->OP)
                             .arg(reference->BPSTART)
                             .arg(reference->TILE));
}

#include "fragmentcontextopen.h"
#include "fragmentpalettes.h"
#include "fragmentpreview.h"
#include "fragmenttilepreview.h"
#include "fragmenttiles.h"
#include "fragmenttilesetproperties.h"
#include "fragmenttilesets.h"
#include "qlayout.h"
#include "ui_fragmentcontextopen.h"

#include <QSplitter>

FragmentContextOpen::FragmentContextOpen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentContextOpen)
{
    ui->setupUi(this);

    QSplitter * splitterH = new QSplitter(this);
    QSplitter * splitterVL = new QSplitter(splitterH);
    QSplitter * splitterVR = new QSplitter(splitterH);

    splitterVL->setOrientation(Qt::Vertical);
    splitterVR->setOrientation(Qt::Vertical);
    splitterH->setOrientation(Qt::Horizontal);

    QWidget * fgTiles = new FragmentTiles(splitterVL);
    QWidget * fgPalettes = new FragmentPalettes(splitterVL);
    QWidget * fgPreview = new FragmentPreview(splitterH);
    QWidget * fgTilePreview = new FragmentTilePreview(splitterVR);
    QWidget * fgTilesets = new FragmentTilesets(splitterVR);
    QWidget * fgTilesetProperties = new FragmentTilesetProperties(splitterVR);

    splitterVL->addWidget(fgTiles);
    splitterVL->addWidget(fgPalettes);

    splitterVR->addWidget(fgTilePreview);
    splitterVR->addWidget(fgTilesets);
    splitterVR->addWidget(fgTilesetProperties);

    splitterH->addWidget(splitterVL);
    splitterH->addWidget(fgPreview);
    splitterH->addWidget(splitterVR);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(splitterH);
    setLayout(layout);

    QList<int> sizesH;
    sizesH << 0.1 * sizeHint().height()
           << 0.8 * sizeHint().height()
           << 0.1 * sizeHint().height();
    splitterH->setSizes(sizesH);
//    splitterH->update();
}

FragmentContextOpen::~FragmentContextOpen()
{
    delete ui;
}

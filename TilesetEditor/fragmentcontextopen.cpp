#include "fragmentcontextopen.h"
#include "fragmenteditor.h"
#include "fragmentpalettes.h"
#include "fragmentreferences.h"
#include "fragmenttilepreview.h"
#include "fragmenttiles.h"
#include "fragmenttilesetproperties.h"
#include "fragmenttilesets.h"
#include "qlayout.h"
#include "ui_fragmentcontextopen.h"

#include <QFile>
#include <QSplitter>

FragmentContextOpen::FragmentContextOpen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentContextOpen)
{
    ui->setupUi(this);

    QString splitterStylesheet = "QSplitter::handle{image: null;}";

    QSplitter * splitterH = new QSplitter(this);
    QSplitter * splitterVL = new QSplitter(splitterH);
    QSplitter * splitterVR = new QSplitter(splitterH);
    QSplitter * splitterVC = new QSplitter(splitterH);

    splitterH->setStyleSheet(splitterStylesheet);
    splitterVL->setStyleSheet(splitterStylesheet);
    splitterVR->setStyleSheet(splitterStylesheet);
    splitterVC->setStyleSheet(splitterStylesheet);

    splitterH->setOrientation(Qt::Horizontal);
    splitterVL->setOrientation(Qt::Vertical);
    splitterVR->setOrientation(Qt::Vertical);
    splitterVC->setOrientation(Qt::Vertical);

    QWidget * fgTiles = new FragmentTiles(splitterVL);
    QWidget * fgPalettes = new FragmentPalettes(splitterVL);
    QWidget * fgReferences = new FragmentReferences(splitterVC);
    QWidget * fgEditor = new FragmentEditor(splitterVC);
    QWidget * fgTilePreview = new FragmentTilePreview(splitterVR);
    QWidget * fgTilesets = new FragmentTilesets(splitterVR);
    QWidget * fgTilesetProperties = new FragmentTilesetProperties(splitterVR);

    splitterVL->addWidget(fgTiles);
    splitterVL->addWidget(fgPalettes);

    splitterVC->addWidget(fgReferences);
    splitterVC->addWidget(fgEditor);

    splitterVR->addWidget(fgTilePreview);
    splitterVR->addWidget(fgTilesets);
    splitterVR->addWidget(fgTilesetProperties);

    splitterH->addWidget(splitterVL);
    splitterH->addWidget(splitterVC);
    splitterH->addWidget(splitterVR);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(splitterH);
    setLayout(layout);


    QList<int> sizesVC;
    sizesVC << 0.4 * sizeHint().height()
           << 0.6 * sizeHint().height();
    splitterVC->setSizes(sizesVC);

    QList<int> sizesH;
    sizesH << 0.1 * sizeHint().width()
           << 0.8 * sizeHint().width()
           << 0.1 * sizeHint().width();
    splitterH->setSizes(sizesH);
}

FragmentContextOpen::~FragmentContextOpen()
{
    delete ui;
}

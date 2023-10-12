#include "app.h"
#include "fragmenteditor.h"
#include "fragmentpreview.h"
#include "fragmentreferences.h"
#include "ui_fragmentpreview.h"

#include <QStackedLayout>

FragmentPreview::FragmentPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPreview)
{
    ui->setupUi(this);

    btEditor = new QPushButton(this);
    btEditor->setText("Editor");

    btReferences = new QPushButton(this);
    btReferences->setText("References");

    auto pagesRow = new QHBoxLayout(this);
    pagesRow->addStretch();
    pagesRow->addWidget(btEditor);
    pagesRow->addWidget(btReferences);
    pagesRow->setContentsMargins(9,0,9,0);


    auto pageControlsLayout = new QVBoxLayout(this);
    pageControlsLayout->addLayout(pagesRow);
    pageControlsLayout->addStretch();
    pageControlsLayout->setContentsMargins(0,0,0,0);

    auto pageControls = new QFrame(this);
    pageControls->setStyleSheet("background-color: transparent");
    pageControls->setLayout(pageControlsLayout);
    pageControls->setContentsMargins(0,0,0,0);

    auto pageContentLayout = new QVBoxLayout();
    pageContentLayout->addWidget(new FragmentEditor(this));
    pageContentLayout->setContentsMargins(0,0,0,0);

    pageContent = new QFrame(this);
//    pageContent->setStyleSheet("background-color: green");
    pageContent->setLayout(pageContentLayout);

    auto stackedLayout = new QStackedLayout(this);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->setContentsMargins(0,0,0,0);

    stackedLayout->addWidget(pageContent);
    stackedLayout->addWidget(pageControls);

    ui->stackedFrame->setLayout(stackedLayout);

    connect(btEditor, &QPushButton::clicked, [&](){ App::getState().setPreviewPage(0); });
    connect(btReferences, &QPushButton::clicked, [&](){ App::getState().setPreviewPage(1); });

    connect(&App::getState(), &AppState::onPreviewPageChanged, [&](int page)
    {
        if (page == 0)
        {
            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentEditor(this));
        }
        else
        {
            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentReferences(this));
        }

        qDebug("Layout has %d widgets", pageContent->layout()->count());
        pageContent->update();
    });

}

FragmentPreview::~FragmentPreview()
{
    delete ui;
}

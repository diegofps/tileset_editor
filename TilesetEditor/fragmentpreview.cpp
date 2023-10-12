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
    pageContent->setLayout(pageContentLayout);

    auto stackedLayout = new QStackedLayout(this);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->setContentsMargins(0,0,0,0);

    stackedLayout->addWidget(pageContent);
    stackedLayout->addWidget(pageControls);

    ui->stackedFrame->setLayout(stackedLayout);

    // Configure Editor and References page buttons

    if (App::getState()->previewPage() == "editor")
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_checked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (App::getState()->previewPage() == "references")
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    connect(btEditor, &QPushButton::clicked, this, [&](){ App::getState()->setPreviewPage("editor"); });
    connect(btReferences, &QPushButton::clicked, this, [&](){ App::getState()->setPreviewPage("references"); });

    connect(App::getState(), &AppState::onPreviewPageChanged, this, [&](QString const & page)
    {
        if (page == "editor")
        {
            btEditor->setStyleSheet(App::getStyles()->get("button_checked"));
            btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));

            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentEditor(this));
        }
        else if (page == "references")
        {
            btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
            btReferences->setStyleSheet(App::getStyles()->get("button_checked"));

            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentReferences(this));
        }
        else
        {
            btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
            btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));
        }

//        pageContent->update();
    });

}

FragmentPreview::~FragmentPreview()
{
    delete ui;
}

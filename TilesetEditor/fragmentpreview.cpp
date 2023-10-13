#include "app.h"
#include "fragmenteditor.h"
#include "fragmentpreview.h"
#include "fragmentreferences.h"
#include "layoutabsolute.h"
#include "ui_fragmentpreview.h"

#include <QStackedLayout>
#include <QMouseEvent>
#include <QLayout>

void FragmentPreview::styleButtons(QString const & value)
{
    if (value == "editor")
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_checked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }
    else if (value == "references")
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_checked"));
    }
    else
    {
        btEditor->setStyleSheet(App::getStyles()->get("button_unchecked"));
        btReferences->setStyleSheet(App::getStyles()->get("button_unchecked"));
    }

    btEditor->update();
    btReferences->update();

}
FragmentPreview::FragmentPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentPreview)
{
    ui->setupUi(this);

    btEditor = new QPushButton(this);
    btEditor->setText("Editor");

    btReferences = new QPushButton(this);
    btReferences->setText("References");

    auto pagesRow = new QHBoxLayout();
    pagesRow->addStretch();
    pagesRow->addWidget(btEditor);
    pagesRow->addWidget(btReferences);
    pagesRow->setContentsMargins(9,0,9,0);
    pagesRow->setSpacing(6);

    auto pageContentLayout = new QVBoxLayout();
    pageContentLayout->addWidget(new FragmentEditor(this));
    pageContentLayout->setContentsMargins(0,0,0,0);

    pageContent = new QFrame(this);
    pageContent->setLayout(pageContentLayout);

    auto stackedLayout = new LayoutAbsolute();
    stackedLayout->setContentsMargins(0,0,0,0);

    stackedLayout->addWidget(pageContent, QRect(0,0,-1,-1));
    stackedLayout->addLayout(pagesRow, QRect(-300,0,300,20));

    ui->stackedFrame->setLayout(stackedLayout);

    // Configure Editor and References page buttons

    styleButtons(App::getState()->previewPage());

    connect(btEditor, &QPushButton::clicked, this, [&](){ App::getState()->setPreviewPage("editor"); });
    connect(btReferences, &QPushButton::clicked, this, [&](){ App::getState()->setPreviewPage("references"); });

    connect(App::getState(), &AppState::onPreviewPageChanged, this, [&](QString const & page)
    {
        styleButtons(page);

        if (page == "editor")
        {
            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentEditor(this));
        }
        else if (page == "references")
        {
            pageContent->layout()->replaceWidget(
                        pageContent->layout()->itemAt(0)->widget(),
                        new FragmentReferences(this));
        }
    });

}

FragmentPreview::~FragmentPreview()
{
    delete ui;
}

#include "app.h"
#include "fragmenteditor.h"
#include "fragmentpreview.h"
#include "fragmentreferences.h"
#include "qabsolutelayout.h"
#include "qframelayout.h"
#include "ui_fragmentpreview.h"

#include <QStackedLayout>
#include <QMouseEvent>
#include <QLayout>

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

//    auto pageControlsLayout = new QVBoxLayout();
//    pageControlsLayout->addLayout(pagesRow);
//    pageControlsLayout->addStretch();
//    pageControlsLayout->setContentsMargins(0,0,0,0);

//    pageControls = new QFrame(this);
//    pageControls->setStyleSheet("background-color: transparent");
//    pageControls->setLayout(pageControlsLayout);
//    pageControls->setContentsMargins(0,0,0,0);
////    pageControls->setAttribute(Qt::WA_TransparentForMouseEvents);
////    pageControls->installEventFilter(this);
////    pageControls->setMouseTracking(true);

    auto pageContentLayout = new QVBoxLayout();
    pageContentLayout->addWidget(new FragmentEditor(this));
    pageContentLayout->setContentsMargins(0,0,0,0);

    pageContent = new QFrame(this);
    pageContent->setLayout(pageContentLayout);

    auto stackedLayout = new QAbsoluteLayout();
//    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->setContentsMargins(0,0,0,0);

    stackedLayout->addWidget(pageContent, QRect(0,0,-1,-1));
    stackedLayout->addLayout(pagesRow, QRect(-300,0,300,20));

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

//bool forwardEvent(QWidget * parent, QMouseEvent * mouseEvent, int depth);

//bool checkLayout(QLayout * layout, QWidget * parent, QMouseEvent * mouseEvent, int const depth, bool & layoutResult)
//{
//    if (layout == nullptr || layout->isEmpty())
//        return false;

//    qDebug("%d: Layout has items, iterating", depth);
//    QPointF mousePos = mouseEvent->globalPosition();

//    for (int i=0;i!=layout->count();++i)
//    {
//        QWidget * widget = layout->itemAt(i)->widget();
//        if (widget != nullptr)
//        {
//            QPoint globalPos = parent->mapToGlobal(widget->pos());
//            QRectF globalRect(globalPos.x(), globalPos.y(), parent->width(), parent->height());

//            if (globalRect.contains(mousePos))
//            {
//                qDebug("%d: Child widget contains the point", depth);

//                bool childResult = forwardEvent(widget, mouseEvent, depth+1);

//                if (childResult)
//                    qDebug("%d: Child widget returned true, returning", depth);
//                else
//                    qDebug("%d: Child widget returned false", depth);

//                layoutResult = childResult;
//                return true;
//            }
//            continue;
//        }

//        QLayout * layout2 = layout->itemAt(i)->layout();
//        if (layout2 != nullptr)
//        {
//            if (checkLayout(layout2, parent, mouseEvent, depth, layoutResult))
//                return true;
//        }

//        qDebug("This is something else, skipping");
//    }

//    return false;
//}

//bool forwardEvent(QWidget * parent, QMouseEvent * mouseEvent, int depth)
//{
//    qDebug("%d: Entering depth", depth);

//    bool layoutResult;
//    if (checkLayout(parent->layout(), parent, mouseEvent, depth, layoutResult))
//    {
//        return layoutResult;
//    }
//    else
//    {
//        QMouseEvent me(mouseEvent->type(),
//                       parent->mapFromGlobal(mouseEvent->globalPosition()),
//                       mouseEvent->globalPosition(),
//                       mouseEvent->button(),
//                       mouseEvent->buttons(),
//                       mouseEvent->modifiers());

//        bool selfResult = qApp->notify(parent, &me);

//        qDebug("%d: Parent widget returned %s - %d %d - %d %d", depth, selfResult ? "true" : "false",
//               me.pos().x(), me.pos().y(),
//               parent->pos().x(), parent->pos().y());

//        return selfResult;
//    }
//}

//bool FragmentPreview::eventFilter(QObject *object, QEvent *event)
//{
//    if (object == pageControls && (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease))
//    {
//        QMouseEvent * mouseEvent = static_cast<QMouseEvent*>(event);
//        qDebug("This is a mouse event %d %d", mouseEvent->pos().x(), mouseEvent->pos().y());
//        return forwardEvent(pageContent, mouseEvent, 1);
//    }

//    return false;

////    if (object == target && event->type() == QEvent::KeyPress) {
////        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
////        if (keyEvent->key() == Qt::Key_Tab) {
////            // Special tab handling
////            return true;
////        } else
////            return false;
////    }
////    return false;
//}

FragmentPreview::~FragmentPreview()
{
    delete ui;
}

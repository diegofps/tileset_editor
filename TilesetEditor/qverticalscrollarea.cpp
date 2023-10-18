#include "qverticalscrollarea.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QEvent>

QVerticalScrollArea::QVerticalScrollArea(QWidget * content, QWidget * parent)
    : QScrollArea(parent),
    _content(content)
{
    setWidgetResizable(true);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    content->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    content->installEventFilter(this);
    setWidget(content);
}

bool QVerticalScrollArea::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == _content && event->type() == QEvent::Resize)
        setMinimumWidth(_content->minimumSizeHint().width()+verticalScrollBar()->width());
    return false;
}



#include "widgetvscrollarea.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QEvent>

WidgetVScrollArea::WidgetVScrollArea(QWidget * content, QWidget * parent)
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

bool WidgetVScrollArea::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == _content && event->type() == QEvent::Resize)
        setMinimumWidth(_content->minimumSizeHint().width()+verticalScrollBar()->width());
    return false;
}



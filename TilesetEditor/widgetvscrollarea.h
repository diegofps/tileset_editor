#ifndef WIDGETVSCROLLAREA_H
#define WIDGETVSCROLLAREA_H

#include <QScrollArea>

class WidgetVScrollArea : public QScrollArea
{
public:
    WidgetVScrollArea(QWidget * content, QWidget * parent = nullptr);

private:
    QWidget * _content;

public:

    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // WIDGETVSCROLLAREA_H

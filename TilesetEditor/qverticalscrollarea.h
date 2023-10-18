#ifndef QVERTICALSCROLLAREA_H
#define QVERTICALSCROLLAREA_H

#include <QScrollArea>

class QVerticalScrollArea : public QScrollArea
{
public:
    QVerticalScrollArea(QWidget * content, QWidget * parent = nullptr);

private:
    QWidget * _content;

public:

    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // QVERTICALSCROLLAREA_H

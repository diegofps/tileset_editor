#ifndef WIDGETPREVIEW_H
#define WIDGETPREVIEW_H

#include <QWidget>
#include <QPen>

class WidgetPreview : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPreview(QWidget *parent = nullptr);


signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    QPen _pen;
    QBrush _brush;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WIDGETPREVIEW_H

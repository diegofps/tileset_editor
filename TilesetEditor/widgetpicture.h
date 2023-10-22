#ifndef WIDGETPICTURE_H
#define WIDGETPICTURE_H

#include <QWidget>

class WidgetPicture : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPicture(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void setPixmap(QPixmap * pixmap);

private:
    void repack();

private:
    QPixmap * _pixmap;
    QRect _rect;

};

#endif // WIDGETPICTURE_H

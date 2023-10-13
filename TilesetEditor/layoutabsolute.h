#ifndef LAYOUTABSOLUTE_H
#define LAYOUTABSOLUTE_H

#include <QLayout>

class LayoutAbsolute : public QLayout
{
private:

    QList<QLayoutItem *> m_items;
    QList<QRect> m_rects;

private:

    void addItem(QLayoutItem * item) override;

public:

    LayoutAbsolute(QWidget * parent = nullptr);
    virtual ~LayoutAbsolute();

    void addWidget(QWidget * widget, QRect rect);
    void addLayout(QLayout * layout, QRect rect);
    QSize sizeHint() const override;
    QSize minimumSize() const override;
    int count() const override;
    QLayoutItem *itemAt(int idx) const override;
    QLayoutItem *takeAt(int idx) override;
    void setGeometry(const QRect &rect) override;

};

#endif // LAYOUTABSOLUTE_H

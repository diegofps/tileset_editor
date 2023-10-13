#ifndef QABSOLUTELAYOUT_H
#define QABSOLUTELAYOUT_H

#include <QLayout>

class QAbsoluteLayout : public QLayout
{
private:

    QList<QLayoutItem *> m_items;
    QList<QRect> m_rects;

    void addItem(QLayoutItem * item) override
    {
//        QLayout::addItem(item);
        m_items.append(item);
    }

public:

    QAbsoluteLayout(QWidget * parent = nullptr) : QLayout(parent)
    {

    }

    virtual ~QAbsoluteLayout()
    {
        for (auto item : m_items)
            delete item;
        m_items.clear();
    }

    void addWidget(QWidget * widget, QRect rect)
    {
        addItem(new QWidgetItem(widget));
        m_rects.append(rect);
    }

    void addLayout(QLayout * layout, QRect rect)
    {
        addItem(layout);
        m_rects.append(rect);
    }

    QSize sizeHint() const override
    {
        QSize s(0, 0);
        for (auto item : m_items)
            s = s.expandedTo(item->sizeHint());
        return s;
    }

    QSize minimumSize() const override
    {
        QSize s(0, 0);
        for (auto item : m_items)
            s = s.expandedTo(item->minimumSize());
        return s;
    }

    int count() const override
    {
        return m_items.size();
    }

    QLayoutItem *itemAt(int idx) const override
    {
        return m_items.value(idx);
    }

    QLayoutItem *takeAt(int idx) override
    {
        return idx >= 0 && idx < m_items.size() ? m_items.takeAt(idx) : 0;
    }

    void setGeometry(const QRect &rect) override
    {
        QLayout::setGeometry(rect);

        if (m_items.size() == 0)
            return;

        for (int i=0;i!=m_items.size();++i)
        {
            auto   item = m_items[i];
            auto & pref = m_rects[i];

            int x,y,w,h;

            if (pref.width() < 0)
            {
                x = 0;
                w = rect.width();
            }
            else
            {
                x = pref.x() < 0 ? rect.width() + pref.x() : pref.x();
                w = pref.width();
            }

            if (pref.height() < 0)
            {
                y = 0;
                h = rect.height();
            }
            else
            {
                y = pref.y() < 0 ? rect.height() + pref.y() : pref.y();
                h = pref.height();
            }

            item->setGeometry(QRect(x,y,w,h));
        }
    }

};

#endif // QABSOLUTELAYOUT_H

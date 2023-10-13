#include "layoutabsolute.h"

void LayoutAbsolute::addItem(QLayoutItem *item)
{
    m_items.append(item);
}

LayoutAbsolute::LayoutAbsolute(QWidget *parent) : QLayout(parent)
{

}

LayoutAbsolute::~LayoutAbsolute()
{
    for (auto item : m_items)
        delete item;
    m_items.clear();
}

void LayoutAbsolute::addWidget(QWidget *widget, QRect rect)
{
    addItem(new QWidgetItem(widget));
    m_rects.append(rect);
}

void LayoutAbsolute::addLayout(QLayout *layout, QRect rect)
{
    addItem(layout);
    m_rects.append(rect);
}

QSize LayoutAbsolute::sizeHint() const
{
    QSize s(0, 0);
    for (auto item : m_items)
        s = s.expandedTo(item->sizeHint());
    return s;
}

QSize LayoutAbsolute::minimumSize() const
{
    QSize s(0, 0);
    for (auto item : m_items)
        s = s.expandedTo(item->minimumSize());
    return s;
}

int LayoutAbsolute::count() const
{
    return m_items.size();
}

QLayoutItem *LayoutAbsolute::itemAt(int idx) const
{
    return m_items.value(idx);
}

QLayoutItem *LayoutAbsolute::takeAt(int idx)
{
    return idx >= 0 && idx < m_items.size() ? m_items.takeAt(idx) : 0;
}

void LayoutAbsolute::setGeometry(const QRect &rect)
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

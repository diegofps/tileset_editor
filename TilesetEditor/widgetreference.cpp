#include "widgetreference.h"
#include <QPainter>

WidgetReference::WidgetReference(QWidget *parent)
    : QWidget{parent},

    _pixmap(nullptr),
    _img(nullptr),
    _offsetImageBuffer(8,8, QImage::Format_ARGB32),
    _offsetImage(nullptr),
    _root(0,0,8,8),
    _offset(0,0,8,8),
    _viewport(0,0,0,0),
    _viewportPower(7)
{
    _brushRoot.setColor(QColor::fromString("#66ffffff"));
    _brushRoot.setStyle(Qt::SolidPattern);

    _brushOffset.setColor(QColor::fromString("#660000ff"));
    _brushOffset.setStyle(Qt::SolidPattern);

    updateViewport();
    update();
}

QImage *WidgetReference::offsetImage()
{
    return _offsetImage;
}

void WidgetReference::setImage(QImage *img)
{
    if (_img != nullptr)
        delete _img;

    if (_pixmap != nullptr)
        delete _pixmap;

    _img = img;
    _pixmap = img == nullptr?nullptr:new QPixmap(QPixmap::fromImage(*_img));

    updateViewport();
    update();
}

void WidgetReference::setRoot(int x, int y)
{
    int const offX = _offset.x() - _root.x();
    int const offY = _offset.y() - _root.y();

    _root.setRect(x, y, 8, 8);
    _offset.setRect(x+offX, y+offY, 8, 8);

    updateOffsetImage();
    updateViewport();
    update();
}

void WidgetReference::setOffset(int rx, int ry)
{
//    qDebug() << "setOffset|Offset:" << _offset;
    _offset.setRect(_root.x()+rx, _root.y()+ry, 8, 8);
    updateOffsetImage();
    update();
}

void WidgetReference::setZoom(int value)
{
    _viewportPower = value;
    updateViewport();
    update();
}

inline void
drawRectangleInViewport(QRect const & rect,
                        QSize const & painterSize,
                        QRect const & viewport,
                        QBrush const & brush,
                        QPainter & painter)
{
#define REPROJECT(x,w,vw) (x)*(w)/(vw)

    int const x1 = REPROJECT(rect.x()-viewport.x(), painterSize.width(), viewport.width());
    int const y1 = REPROJECT(rect.y()-viewport.y(), painterSize.height(), viewport.height());
    int const x2 = REPROJECT(rect.x()+rect.width()-viewport.x(), painterSize.width(), viewport.width());
    int const y2 = REPROJECT(rect.y()+rect.height()-viewport.y(), painterSize.height(), viewport.height());

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRect(QRect(x1,y1,x2-x1,y2-y1));
}

void WidgetReference::paintEvent(QPaintEvent *event)
{
    (void) event;
    QPainter painter(this);

    if (_pixmap == nullptr || _viewport.width() == 0 || _viewport.height() == 0)
        return;

    painter.drawPixmap(QRect(0,0,width(),height()), *_pixmap, _viewport);
    drawRectangleInViewport(_root, size(), _viewport, _brushRoot, painter);
    drawRectangleInViewport(_offset, size(), _viewport, _brushOffset, painter);
}

void WidgetReference::resizeEvent(QResizeEvent *event)
{
    (void) event;
    updateViewport();
    update();
}

void WidgetReference::updateOffsetImage()
{
    _offsetImage = nullptr;

    if (_img == nullptr)
        return;

    if (_offset.x() < 0 || _offset.x()+_offset.width() > _img->width())
        return;

    if (_offset.y() < 0 || _offset.y()+_offset.height() > _img->height())
        return;

    _offsetImage = &_offsetImageBuffer;

    for (int i=0;i!=_offsetImageBuffer.height();++i)
    {
        QRgb * srcLine = reinterpret_cast<QRgb*>(_img->scanLine(i));
        QRgb * dstLine = reinterpret_cast<QRgb*>(_offsetImageBuffer.scanLine(i));

        for (int j=0;j!=_offsetImageBuffer.width();++j)
            dstLine[i] = srcLine[i];
    }
}

void WidgetReference::updateViewport()
{
    if (_img == nullptr)
    {
        _viewport = QRect(0,0,0,0);
        return;
    }

    int const fw = width();
    int const fh = height();

    int const w = std::pow(1.4, _viewportPower);
    int const h = w * fh / fw;

    int x = 0;
    int y = 0;

    if (w >= _img->width())
    {
        qDebug() << 1.0 << QRect(x,y,w,h) << QPoint(_img->width(),_img->height());
        x = -(w-_img->width())/2;
    }
    else
    {
        qDebug() << 2.0 << QRect(x,y,w,h) << QPoint(_img->width(),_img->height());
        x = _root.x() - w / 2;

        if (x < 0 && x + w < _img->width())
            x = 0;

        if (x > 0 && x + w > _img->width())
            x = _img->width() - w;
    }

    if (h >= _img->height())
    {
        qDebug() << 3.0 << QRect(x,y,w,h) << QPoint(_img->width(),_img->height());
        y = -(h-_img->height())/2;
    }
    else
    {
        qDebug() << 4.0 << QRect(x,y,w,h) << QPoint(_img->width(),_img->height());
        y = _root.y() - h / 2;

        if (y < 0 && y + h < _img->height())
            y = 0;

        if (y > 0 && y + h > _img->height())
            y = _img->height() - h;
    }

    qDebug() << 5.0 << QRect(x,y,w,h) << QPoint(_img->width(),_img->height()) << "\n";
    _viewport.setRect(x, y, w, h);
}

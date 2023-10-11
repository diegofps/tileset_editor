#ifndef FRAGMENTPREVIEW_H
#define FRAGMENTPREVIEW_H

#include <QWidget>

namespace Ui {
class FragmentPreview;
}

class FragmentPreview : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentPreview(QWidget *parent = nullptr);
    ~FragmentPreview();

private:
    Ui::FragmentPreview *ui;
};

#endif // FRAGMENTPREVIEW_H

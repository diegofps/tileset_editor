#ifndef FRAGMENTPREVIEW_H
#define FRAGMENTPREVIEW_H

#include <QFrame>
#include <QPushButton>
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
    QPushButton * btEditor;
    QPushButton * btReferences;
    QFrame * pageControls;
    QFrame * pageContent;
};

#endif // FRAGMENTPREVIEW_H

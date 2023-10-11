#ifndef FRAGMENTCONTEXTCLOSED_H
#define FRAGMENTCONTEXTCLOSED_H

#include <QWidget>

namespace Ui {
class FragmentContextClosed;
}

class FragmentContextClosed : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentContextClosed(QWidget *parent = nullptr);
    ~FragmentContextClosed();

private:
    Ui::FragmentContextClosed *ui;
};

#endif // FRAGMENTCONTEXTCLOSED_H

#ifndef FRAGMENTCONTEXTOPEN_H
#define FRAGMENTCONTEXTOPEN_H

#include <QWidget>

namespace Ui {
class FragmentContextOpen;
}

class FragmentContextOpen : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentContextOpen(QWidget *parent = nullptr);
    ~FragmentContextOpen();

private:
    Ui::FragmentContextOpen *ui;
};

#endif // FRAGMENTCONTEXTOPEN_H

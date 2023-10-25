#ifndef FRAGMENTREFERENCES_H
#define FRAGMENTREFERENCES_H

#include "appstate.h"
#include <QWidget>

namespace Ui {
class FragmentReferences;
}

class FragmentReferences : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentReferences(QWidget *parent = nullptr);
    ~FragmentReferences();

private:
    void styleScreenshotButtons(ReferenceMode const value);
    void styleHighlightPositionButton(const bool value);
    void updateReferenceWidget(const QList<Tile *> *tiles, const ReferenceMode value);

private:
    Ui::FragmentReferences *ui;
};

#endif // FRAGMENTREFERENCES_H

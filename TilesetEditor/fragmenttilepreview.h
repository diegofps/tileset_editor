#ifndef FRAGMENTTILEPREVIEW_H
#define FRAGMENTTILEPREVIEW_H

#include "model/palette.h"
#include <QWidget>

namespace Ui {
class FragmentTilePreview;
}

class FragmentTilePreview : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentTilePreview(QWidget *parent = nullptr);
    ~FragmentTilePreview();

private:
    void styleButtons(QString const & value);
    void loadPalette(Palette * value);

private:
    Ui::FragmentTilePreview *ui;
};

#endif // FRAGMENTTILEPREVIEW_H

#ifndef FRAGMENTTILEPREVIEW_H
#define FRAGMENTTILEPREVIEW_H

#include "appstate.h"
#include "model/palette.h"
#include "model/tile.h"
#include <QPushButton>
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
    void updateTileWidget(TileMode const & mode, QList<Tile*> const * tiles, Palette * palette);
    void styleButton(QPushButton *btn, bool value);
    void updatePaletteWidget(Palette * value, QList<Tile*> const * tiles);

private:
    Ui::FragmentTilePreview *ui;
};

#endif // FRAGMENTTILEPREVIEW_H

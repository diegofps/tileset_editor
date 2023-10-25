#ifndef FRAGMENTPALETTES_H
#define FRAGMENTPALETTES_H

#include "appstate.h"
#include "model/palette.h"
#include "model/tile.h"
#include <QWidget>

namespace Ui {
class FragmentPalettes;
}

class FragmentPalettes : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentPalettes(QWidget *parent = nullptr);
    ~FragmentPalettes();

private:
    void filterPalettes(QList<Tile*> const * selectedTiles, QList<Palette*> const * value, PaletteMode paletteMode);
    void styleButtons(PaletteMode value);
    void updatePalettesWidget(QList<Tile*> const * tiles);
    void saveSelectedPalette();
    void restoreSelectedPalette(QList<Tile*> const * selectedTiles);

private:
    Ui::FragmentPalettes *ui;
    QList<Palette*> _palettes;
    int _lastSelectedItemID;
};

#endif // FRAGMENTPALETTES_H

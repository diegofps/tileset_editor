#ifndef FRAGMENTTILESETS_H
#define FRAGMENTTILESETS_H

#include "model/tileset.h"
#include <QWidget>

namespace Ui {
class FragmentTilesets;
}

class FragmentTilesets : public QWidget
{
    Q_OBJECT

public:
    explicit FragmentTilesets(QWidget *parent = nullptr);
    ~FragmentTilesets();

private:

    void loadTilesets(QList<Tileset*> const * value);

private:
    Ui::FragmentTilesets *ui;
};

#endif // FRAGMENTTILESETS_H

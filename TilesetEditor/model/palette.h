#ifndef PALETTE_H
#define PALETTE_H

#include <QJsonObject>
#include <cstddef>

class Palette
{
private:

    size_t _id;
    int _colors[256];  // A copy of the first colorPalette used
    int _size;         // The number of colors in this color palette (up to 256)
    int _frequency;

public:

    Palette(QJsonObject data);

};

#endif // PALETTE_H

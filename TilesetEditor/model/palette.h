#ifndef PALETTE_H
#define PALETTE_H

#include <QJsonObject>
#include <cstddef>
#include <QColor>

class Palette
{
private:

    QByteArray _uniqueKey;

public:

    int id;
    int size;         // The number of colors in this color palette (up to 256)
    int frequency;
    QColor colors[256];  // A copy of the first colorPalette used

public:

    Palette(QJsonObject & data);
    QJsonObject exportAsJson();
    QByteArray & uniqueKey();
//    void import(Palette * other);

};

#endif // PALETTE_H

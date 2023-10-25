#ifndef CELL_H
#define CELL_H

#include <QJsonObject>

class Cell
{
public:

    int id;
    int tileID;
    int paletteID;
    int x;
    int y;
    bool hFlip;
    bool vFlip;
    bool isLink;

public:

    Cell(QJsonObject & data);
    QJsonObject exportAsJson();

};

#endif // CELL_H

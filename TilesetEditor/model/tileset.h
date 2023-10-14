#ifndef TILESET_H
#define TILESET_H

#include <QJsonObject>
#include <cstddef>
#include <QColor>

class Tileset
{
private:

    size_t _id;
    int _gridW;
    int _gridH;
    QColor _bgColor;

public:

    Tileset(QJsonObject data);
    QJsonObject exportAsJson();
    QString uniqueKey();
    void import(Tileset * other);

};

#endif // TILESET_H

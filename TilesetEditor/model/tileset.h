#ifndef TILESET_H
#define TILESET_H

#include <QJsonObject>
#include <cstddef>
#include <QColor>

class Tileset
{
public:

    int id;
    QString name;
    QColor bgColor;
    int gridW;
    int gridH;

public:

    Tileset();
    Tileset(QJsonObject data);
    QJsonObject exportAsJson();
//    QByteArray & uniqueKey();

};

#endif // TILESET_H

#ifndef TILESET_H
#define TILESET_H

#include "model/cell.h"
#include <QJsonObject>
#include <cstddef>
#include <QColor>
#include <QHash>
#include <QPair>

class Tileset
{
public:

    int id;
    int sceneId;
    QString name;
    QColor bgColor;
    int gridW;
    int gridH;

    QHash<QPair<int,int>,Cell*> cells;

public:

    Tileset();
    Tileset(QJsonObject data);
    ~Tileset();
    QJsonObject exportAsJson();
//    QByteArray & uniqueKey();

};

#endif // TILESET_H

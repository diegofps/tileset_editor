#ifndef TILESET_H
#define TILESET_H

#include "model/cell.h"
#include <QJsonObject>
#include <cstddef>
#include <QColor>
#include <QHash>
#include <QPair>

class CellCommand;
class AppState;

class Tileset
{
public:

    int id;
    int sceneId;
    int gridW;
    int gridH;
    QString name;
    QColor bgColor;
    QHash<QPair<int,int>,Cell*> cells;

    QList<CellCommand*> _history;
    qsizetype _historyPosition;

public:

    Tileset();
    Tileset(QJsonObject data);
    ~Tileset();
    QJsonObject exportAsJson();
//    QByteArray & uniqueKey();

    void historyClear();
    bool historyAdd(AppState * state, CellCommand * cmd);
    bool historyUndo(AppState * state);
    bool historyRedo(AppState * state);

};

#endif // TILESET_H

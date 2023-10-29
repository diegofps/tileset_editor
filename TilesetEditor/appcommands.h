#ifndef APPCOMMANDS_H
#define APPCOMMANDS_H

#include "appstate.h"
#include "commands/command.h"

class AppCommands
{
public:

    AppCommands(AppState * _state);

    void setTilesFilter(TilesFilter const & filter);
    void setSelectedScenePos(int scenePos);
    void setSelectedTilesPos(Range range);

    void historyClear();
    void historyUndo();
    void historyRedo();

private:

    void historyAdd(Command * cmd);

private:

    AppState * _state;
    QList<Command*> _history;
    qsizetype _historyPosition;

};

#endif // APPCOMMANDS_H

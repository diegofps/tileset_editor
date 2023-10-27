#include "appcommands.h"
#include "commands/tilesfiltercommand.h"
#include "commands/selectsceneposcommand.h"
#include "commands/setselectedtilesposcommand.h"


AppCommands::AppCommands(AppState * state) :
    _state(state),
    _historyPosition(0)
{

}

void AppCommands::setSelectedScenePos(int scenePos)
{
    if (_state->selectedScenePos() != scenePos)
        historyAdd(new SelectScenePosCommand(_state->selectedScenePos(), scenePos));
}

void AppCommands::setSelectedTilesPos(Range range)
{
    if (*_state->selectedTilesPos() != range)
        historyAdd(new SetSelectedTilesPosCommand(*_state->selectedTilesPos(), range));
}

void AppCommands::setTilesFilter(TilesFilter const & filter)
{
    if (_state->tilesFilter() != filter)
        historyAdd(new TilesFilterCommand(_state->tilesFilter(), filter));
}

void AppCommands::historyClear()
{
    for (auto cmd : _history)
        delete cmd;
    _history.clear();
    _historyPosition = 0;
}

void AppCommands::historyUndo()
{
    if (_historyPosition != 0)
    {
        --_historyPosition;
        _history[_historyPosition]->unexecute(*_state);
    }
}

void AppCommands::historyRedo()
{
    if (_historyPosition != _history.size())
    {
        _history[_historyPosition]->execute(*_state);
        ++_historyPosition;
    }
}

void AppCommands::historyAdd(Command * cmd)
{
    for (qsizetype tmp=_historyPosition;tmp!=_history.size();++tmp)
        delete _history[tmp];

    _history.resize(_historyPosition);

    if (_historyPosition == 0)
    {
        _history.append(cmd);
        ++_historyPosition;
        cmd->execute(*_state);
    }
    else
    {
        if (_history[_historyPosition-1]->tryMerge(cmd))
        {
            cmd->execute(*_state);
            delete cmd;
        }
        else
        {
            _history.append(cmd);
            ++_historyPosition;
            cmd->execute(*_state);
        }
    }
}

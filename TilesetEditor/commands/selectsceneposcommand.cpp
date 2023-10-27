#include "selectsceneposcommand.h"

SelectScenePosCommand::SelectScenePosCommand(int prev, int next) :
    Command("SelectedScenePos"),

    _prev(prev),
    _next(next)
{

}

void SelectScenePosCommand::execute(AppState &_state)
{
    _state.setSelectedScenePos(_next);
}

void SelectScenePosCommand::unexecute(AppState &_state)
{
    _state.setSelectedScenePos(_prev);
}

bool SelectScenePosCommand::tryMerge(const Command *other)
{
    (void)other;
    return false;
}

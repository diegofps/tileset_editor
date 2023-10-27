#include "setselectedtilesposcommand.h"

SetSelectedTilesPosCommand::SetSelectedTilesPosCommand(Range const & prev, Range const & next) :
    Command("SetSelectedTiles"),

    _prev(prev),
    _next(next)
{

}

void SetSelectedTilesPosCommand::execute(AppState & _state)
{
    _state.setSelectedTilesPos(_next);
}

void SetSelectedTilesPosCommand::unexecute(AppState &_state)
{
    _state.setSelectedTilesPos(_prev);
}

bool SetSelectedTilesPosCommand::tryMerge(const Command *other)
{
    (void)other;
    return false;
}

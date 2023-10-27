#include "tilesfiltercommand.h"


TilesFilterCommand::TilesFilterCommand(const TilesFilter &prev, const TilesFilter &next) :
    Command("TilesFilter"),

    _prev(prev),
    _next(next)
{

}

void TilesFilterCommand::execute(AppState &_state)
{
    _state.setTilesFilter(_next);
}

void TilesFilterCommand::unexecute(AppState &_state)
{
    _state.setTilesFilter(_prev);
}

bool TilesFilterCommand::tryMerge(const Command *other)
{
    if (name() != other->name())
        return false;

    auto other2 = static_cast<TilesFilterCommand const*>(other);
    _next = other2->_next;
    return true;
}

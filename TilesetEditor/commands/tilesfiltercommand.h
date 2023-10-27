#ifndef TILESFILTERCOMMAND_H
#define TILESFILTERCOMMAND_H

#include "commands/command.h"

class TilesFilterCommand : public Command
{
public:

    TilesFilterCommand(TilesFilter const & prev, TilesFilter const & next);
    virtual void execute(AppState & _state);
    virtual void unexecute(AppState & _state);
    virtual bool tryMerge(Command const * other);

private:

    TilesFilter _prev;
    TilesFilter _next;

};

#endif // TILESFILTERCOMMAND_H

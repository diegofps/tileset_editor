#ifndef SETSELECTEDTILESPOSCOMMAND_H
#define SETSELECTEDTILESPOSCOMMAND_H

#include "command.h"

class SetSelectedTilesPosCommand : public Command
{
public:

    SetSelectedTilesPosCommand(Range const & prev, Range const & next);
    virtual void execute(AppState & _state);
    virtual void unexecute(AppState & _state);
    virtual bool tryMerge(Command const * other);

private:

    Range _prev;
    Range _next;

};


#endif // SETSELECTEDTILESPOSCOMMAND_H

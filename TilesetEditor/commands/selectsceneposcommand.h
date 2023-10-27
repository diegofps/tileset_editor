#ifndef SELECTSCENEPOSCOMMAND_H
#define SELECTSCENEPOSCOMMAND_H

#include "commands/command.h"

class SelectScenePosCommand : public Command
{
public:

    SelectScenePosCommand(int prev, int next);
    virtual void execute(AppState & _state);
    virtual void unexecute(AppState & _state);
    virtual bool tryMerge(Command const * other);

private:

    int _prev;
    int _next;

};

#endif // SELECTSCENEPOSCOMMAND_H

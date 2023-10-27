#ifndef COMMAND_H
#define COMMAND_H

#include "appstate.h"
#include <QString>

class Command
{
public:

    Command(char const * name) : _name(name)  { }
    QString const & name() const { return _name; }
    virtual ~Command() { }

    virtual void execute(AppState & _state) = 0;
    virtual void unexecute(AppState & _state) = 0;
    virtual bool tryMerge(Command const * other) = 0;

private:

    QString _name;

};

#endif // COMMAND_H

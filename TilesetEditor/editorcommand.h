#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include "appstate.h"

class CellCommand
{
public:

    CellCommand();
    virtual ~CellCommand() {}
    virtual bool execute(AppState * state) = 0;
    virtual bool unexecute(AppState * state) = 0;

};

class PaintCommand : public CellCommand
{
public:

    PaintCommand(QPair<int,int> const & position, Cell const * prev, Cell const * next, bool wasLinked);
    virtual bool execute(AppState * state);
    virtual bool unexecute(AppState * state);

private:

    QPair<int,int> _position;
    Cell _prev;
    Cell _next;
    bool _wasNullptr;
    bool _becomesNullptr;
    bool _wasLinked;
};

class LinkCommand : public CellCommand
{
public:

    LinkCommand(QPair<int,int> const & position, int previousCellID, int nextCellID);
    virtual bool execute(AppState * state);
    virtual bool unexecute(AppState * state);

private:

    QPair<int,int> _position;
    int _prev;
    int _next;
};

#endif // EDITORCOMMAND_H

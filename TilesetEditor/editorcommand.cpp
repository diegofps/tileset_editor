#include "editorcommand.h"
#include "app.h"

CellCommand::CellCommand()
{

}

PaintCommand::PaintCommand(QPair<int,int> const & position, Cell const * prev, Cell const * next, bool wasLinked) :
    _position(position),
    _wasNullptr(prev==nullptr),
    _becomesNullptr(next==nullptr),
    _wasLinked(wasLinked)
{
    if (prev != nullptr)
        _prev = *prev;

    if (next != nullptr)
        _next = *next;
}

inline bool updateCell(AppState * state,
                       QPair<int,int> const & position,
                       bool const erase,
                       Cell const & newValue,
                       bool link)
{
    if (state == nullptr || state->selectedTileset() == nullptr)
        return false;

    auto & cells = state->selectedTileset()->cells;

    if (erase)
    {
        auto it = cells.constFind(position);
        if (it != cells.constEnd())
        {
            state->notifyCellDeleted(it.value(), 0);
            delete it.value();
            cells.erase(it);
        }
    }
    else
    {
        auto it = cells.constFind(position);
        Cell * cell;

        if (it == cells.constEnd())
        {
            cell = new Cell(newValue);
            cells.insert(position, cell);
            state->notifyCellCreated(cell);
        }
        else
        {
            cell = it.value();
            state->notifyCellDeleted(cell, cell->tileID == newValue.tileID ? newValue.id : 0);
            *cell = newValue;
            state->notifyCellCreated(cell);
        }

        if (link)
        {
            auto tile = App::getState()->getTileById(cell->tileID);
            if (tile != nullptr)
                tile->linkedCellID = cell->id;
        }
    }

//    state->updateFilteredTiles();
    return true;
}

bool PaintCommand::execute(AppState *state)
{
    return updateCell(state, _position, _becomesNullptr, _next, false);
}

bool PaintCommand::unexecute(AppState *state)
{
    return updateCell(state, _position, _wasNullptr, _prev, _wasLinked);
}

LinkCommand::LinkCommand(const QPair<int, int> &position, int previousCellID, int nextCellID) :
    _position(position), _prev(previousCellID), _next(nextCellID)
{

}

inline bool updateLinkedCellID(AppState * state, QPair<int,int> const & position, int const value)
{
    if (state == nullptr || state->selectedTileset() == nullptr)
        return false;

    auto & cells = state->selectedTileset()->cells;

    auto it = cells.find(position);

    if (it == cells.end())
        return false;

    auto tile = App::getState()->getTileById(it.value()->tileID);

    if (tile == nullptr)
        return false;

    tile->linkedCellID = value;
//    App::getState()->updateFilteredTiles();

    return true;
}

bool LinkCommand::execute(AppState *state)
{
    return updateLinkedCellID(state, _position, _next);
}

bool LinkCommand::unexecute(AppState *state)
{
    return updateLinkedCellID(state, _position, _prev);
}

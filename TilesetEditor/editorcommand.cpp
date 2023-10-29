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

inline void updateCell(AppState * state,
                       QPair<int,int> const & position,
                       bool const erase,
                       Cell const & newValue,
                       bool link)
{
    if (state == nullptr || state->selectedTileset() == nullptr)
        return;

    auto & cells = state->selectedTileset()->cells;

    if (erase)
    {
        auto it = cells.constFind(position);
        if (it != cells.constEnd())
        {
            state->notifyCellDeleted(it.value());
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
            state->notifyCellDeleted(cell);
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

    state->updateFilteredTiles();
}

void PaintCommand::execute(AppState *state)
{
    updateCell(state, _position, _becomesNullptr, _next, false);
}

void PaintCommand::unexecute(AppState *state)
{
    updateCell(state, _position, _wasNullptr, _prev, _wasLinked);
}

LinkCommand::LinkCommand(const QPair<int, int> &position, int previousCellID, int nextCellID) :
    _position(position), _prev(previousCellID), _next(nextCellID)
{

}

inline void updateLinkedCellID(AppState * state, QPair<int,int> const & position, int const value)
{
    if (state == nullptr || state->selectedTileset() == nullptr)
        return;

    auto & cells = state->selectedTileset()->cells;

    auto it = cells.find(position);

    if (it == cells.end())
        return;

    auto tile = App::getState()->getTileById(it.value()->tileID);

    if (tile == nullptr)
        return;

    tile->linkedCellID = value;
}

void LinkCommand::execute(AppState *state)
{
    updateLinkedCellID(state, _position, _next);
}

void LinkCommand::unexecute(AppState *state)
{
    updateLinkedCellID(state, _position, _prev);
}

#include "appstate.h"

AppState::AppState()
{

}

// Commands

void AppState::setContextFolder(QString value)
{
    if (value != _contextFolder)
    {
        _contextFolder = value;
        emit onContextFolderChanged(value);
    }
}

const QString &AppState::contextFolder() const
{
    return _contextFolder;
}

// Editor Toolbox

void AppState::setEditorTool(EditorTool value)
{
    if (value != _editorTool)
    {
        _editorTool = value;
        emit onEditorToolChanged(value);
    }
}

void AppState::setEditorShowLinkedTiles(bool value)
{
    if (value != _editorShowLinkedTiles)
    {
        _editorShowLinkedTiles = value;
        emit onEditorShowLinkedTilesChanged(value);
    }
}

void AppState::setEditorShowUnlinkedTiles(bool value)
{
    if (value != _editorShowUnlinkedTiles)
    {
        _editorShowUnlinkedTiles = value;
        emit onEditorShowUnlinkedTilesChanged(value);
    }
}

void AppState::setEditorShowGrid(bool value)
{
    if (value != _editorShowGrid)
    {
        _editorShowGrid = value;
        emit onEditorShowGridChanged(value);
    }
}

EditorTool AppState::editorTool() const
{
    return _editorTool;
}

bool AppState::editorShowLinkedTiles() const
{
    return _editorShowLinkedTiles;
}

bool AppState::editorShowUnlinkedTiles() const
{
    return _editorShowUnlinkedTiles;
}

bool AppState::editorShowGrid() const
{
    return _editorShowGrid;
}

// References Toolbox

void AppState::setReferenceScreenshot(int value)
{
    if (value != _referenceScreenshot)
    {
        _referenceScreenshot = value;
        emit onReferenceScreenshotChanged(value);
    }
}

void AppState::setReferenceHighlightPosition(bool value)
{
    if (value != _referenceHighlightPosition)
    {
        _referenceHighlightPosition = value;
        emit onReferenceHighlightPositionChanged(value);
    }
}

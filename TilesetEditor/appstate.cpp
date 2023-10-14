#include "appstate.h"

AppState::AppState()
{
    _tilesShow = "all";
    _palettesShow = "all";
    _tilePreviewShow = "original";
    _previewPage = "editor";
    _referenceScreenshot = 0;
    _referenceHighlightPosition = true;
    _editorTool = PENCIL;
    _editorShowLinkedTiles = true;
    _editorShowUnlinkedTiles = true;
    _editorShowGrid = true;

    _context = new Context();
    _contextTiles = new QList<Tile*>();
    _contextPalettes = new QList<Palette*>();
    _contextTilesets = new QList<Tileset*>();
}

// Context Folder

void AppState::setContext(Context * value)
{
    if (value != _context)
    {
        _context = value;
        emit onContextChanged(value);
    }
}

void AppState::setContextFolder(QString value)
{
    if (value != _contextFolder)
    {
        _contextFolder = value;
        emit onContextFolderChanged(value);
    }
}

void AppState::setContextTiles(QList<Tile *> *value)
{
    if (value != _contextTiles)
    {
        _contextTiles = value;
        emit onContextTilesChanged(value);
    }
}

void AppState::setContextPalettes(QList<Palette *> *value)
{
    if (value != _contextPalettes)
    {
        _contextPalettes = value;
        emit onContextPalettesChanged(value);
    }
}

void AppState::setContextTilesets(QList<Tileset *> *value)
{
    if (value != _contextTilesets)
    {
        _contextTilesets = value;
        emit onContextTilesetsChanged(value);
    }
}

Context * AppState::context() const
{
    return _context;
}

const QString & AppState::contextFolder() const
{
    return _contextFolder;
}

QList<Tile *> * AppState::contextTiles() const
{
    return _contextTiles;
}

QList<Palette *> * AppState::contextPalettes() const
{
    return _contextPalettes;
}

QList<Tileset *> * AppState::contextTilesets() const
{
    return _contextTilesets;
}


// Editor Toolbox

void AppState::setEditorTool(EditorTool const value)
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

int AppState::referenceScreenshot() const
{
    return _referenceScreenshot;
}

bool AppState::referenceHighlightPosition() const
{
    return _referenceHighlightPosition;
}

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

// Preview

void AppState::setPreviewPage(QString value)
{
    if (value != _previewPage)
    {
        _previewPage = value;
        emit onPreviewPageChanged(value);
    }
}

const QString & AppState::previewPage() const
{
    return _previewPage;
}

// Tiles

void AppState::setTilesShow(QString const & value)
{
    if (value != _tilesShow)
    {
        _tilesShow = value;
        emit onTilesShowChanged(value);
    }
}

const QString & AppState::tilesShow() const
{
    return _tilesShow;
}

// Palettes

void AppState::setPalettesShow(QString const & value)
{
    if (value != _palettesShow)
    {
        _palettesShow = value;
        emit onPalettesShowChanged(value);
    }
}

const QString &AppState::palettesShow() const
{
    return _palettesShow;
}


// TilePreview

void AppState::setTilePreviewShow(QString const & value)
{
    if (value != _tilePreviewShow)
    {
        _tilePreviewShow = value;
        emit onTilePreviewShowChanged(value);
    }
}

const QString &AppState::tilePreviewShow() const
{
    return _tilePreviewShow;
}


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

    _project = nullptr;
    _projectTiles = nullptr;
    _projectPalettes = nullptr;
    _projectTilesets = nullptr;
    _projectReferences = nullptr;
    _projectScreenshots = nullptr;
}

// Context Folder

template <typename ITEM>
void deleteItemsAndQListIfNotNullptr(QList<ITEM*> * oldList, QList<ITEM*> * newList)
{
    if (oldList == nullptr)
        return;

    if (newList == oldList)
        return;

    for (ITEM * v : *oldList)
        delete v;

    delete oldList;
}

void AppState::setProject(Project * value)
{
    if (_project != nullptr && value != _project)
        delete _project;

    _project = value;
    emit onProjectChanged(value);
}

void AppState::setProjectLastDumpFolder(QString value)
{
    if (value != _projectLastDumpFolder)
    {
        _projectLastDumpFolder = value;
        emit onProjectLastDumpFolderChanged(value);
    }
}

void AppState::setProjectTiles(QList<Tile *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTiles, value);
    _projectTiles = value;
    emit onProjectTilesChanged(value);
}

void AppState::setProjectPalettes(QList<Palette *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectPalettes, value);
    _projectPalettes = value;
    emit onProjectPalettesChanged(value);
}

void AppState::setProjectReferences(QList<Reference *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectReferences, value);
    _projectReferences = value;
    emit onProjectReferencesChanged(value);
}

void AppState::setProjectTilesets(QList<Tileset *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTilesets, value);
    _projectTilesets = value;
    emit onProjectTilesetsChanged(value);
}

void AppState::setProjectScreenshots(QList<Screenshot*> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectScreenshots, value);
    _projectScreenshots = value;
    emit onProjectScreenshotsChanged(value);
}

void AppState::insertProjectTileset(int const position, Tileset * value)
{
    if (value == nullptr)
        return;

    _projectTilesets->insert(position, value);
    emit onProjectTilesetsInserted(_projectTilesets, position);
}

void AppState::removeProjectTileset(int const position)
{
    if (position < 0 || position >= _projectTilesets->size())
        return;

    auto ts = _projectTilesets->at(position);
    _projectTilesets->remove(position);
    emit onProjectTilesetsRemoved(_projectTilesets, position);
    delete ts;
}

void AppState::moveUpProjectTileset(int const position)
{
    if (position < 0 || position >= _projectTilesets->size()-1)
        return;

    auto ts = _projectTilesets->takeAt(position);
    _projectTilesets->insert(position+1, ts);

    emit onProjectTilesetsMoved(_projectTilesets, position, position+1);
}

void AppState::moveDownProjectTileset(int const position)
{
    if (position <= 0 || position >= _projectTilesets->size())
        return;

    auto ts = _projectTilesets->takeAt(position);
    _projectTilesets->insert(position-1, ts);

    emit onProjectTilesetsMoved(_projectTilesets, position, position-1);
}

Project * AppState::project() const
{
    return _project;
}

const QString &AppState::projectLastDumpFolder() const
{
    return _projectLastDumpFolder;
}

QList<Tile *> * AppState::projectTiles() const
{
    return _projectTiles;
}

QList<Palette *> * AppState::projectPalettes() const
{
    return _projectPalettes;
}

QList<Reference *> * AppState::projectReferences() const
{
    return _projectReferences;
}

QList<Screenshot *> * AppState::projectScreenshots() const
{
    return _projectScreenshots;
}

QList<Tileset *> * AppState::projectTilesets() const
{
    return _projectTilesets;
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

void AppState::setPalettesSelectedItem(Palette * value)
{
    _palettesSelectedItem = value;
    emit onPaletteSelectedItemChanged(value);
}

const QString &AppState::palettesShow() const
{
    return _palettesShow;
}

Palette * AppState::palettesSelectedItem()
{
    return _palettesSelectedItem;
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

Tileset * AppState::tilesetsSelectedItem() const
{
    return _tilesetsSelectedItem;
}

const QString &AppState::tilePreviewShow() const
{
    return _tilePreviewShow;
}

// Tilesets

void AppState::setTilesetsSelectedItem(Tileset * value)
{
    _tilesetsSelectedItem = value;
    emit onTilesetsSelectedItemChanged(value);
}

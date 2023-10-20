#include "appstate.h"

AppState::AppState()
{
    _tilesetsSelectedItem = nullptr;

    _tilePreviewShow = "original";

    _palettesShow = "all";
    _palettesSelectedItem = nullptr;

    _tilesFilter = new TilesFilter();

    _previewPage = "editor";

    _referenceScreenshot = 0;
    _referenceHighlightPosition = true;

    _editorTool = PENCIL;
    _editorShowLinkedTiles = true;
    _editorShowUnlinkedTiles = true;
    _editorShowGrid = true;

    _project = nullptr;
    _projectHasChanges = false;
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

void AppState::setProjectHasChanges(bool value)
{
    if (value != _projectHasChanges)
    {
        _projectHasChanges = value;
        emit onProjectHasChangesChanged(value);
    }
}

void AppState::setProjectLastDumpFolder(QString value)
{
    if (value != _projectLastDumpFolder)
    {
        _projectLastDumpFolder = value;
        emit onProjectLastDumpFolderChanged(value);
    }
}


template <typename ITEM, typename KEY_FUNC>
void recreateIndex(QList<ITEM*> * value, QHash<int, ITEM*> & index, KEY_FUNC keyFunc)
{
    index.clear();
    if (value != nullptr)
        for (ITEM * t : *value)
            index[keyFunc(t)] = t;
}

void AppState::setProjectTiles(QList<Tile *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTiles, value);
    _projectTiles = value;
    recreateIndex(value, _index_Tile_ID, [](Tile* item){ return item->id; });
    emit onProjectTilesChanged(value);
}

void AppState::setProjectPalettes(QList<Palette *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectPalettes, value);
    _projectPalettes = value;
    recreateIndex(value, _index_Palette_ID, [](Palette* item){ return item->id; });
    emit onProjectPalettesChanged(value);
}

void AppState::setProjectReferences(QList<Reference *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectReferences, value);
    _projectReferences = value;
    recreateIndex(value, _index_Reference_ID, [](Reference * item){ return item->id; });
    emit onProjectReferencesChanged(value);
}

void AppState::setProjectTilesets(QList<Tileset *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTilesets, value);
    _projectTilesets = value;
    recreateIndex(value, _index_Tileset_ID, [](Tileset* item){ return item->id; });
    emit onProjectTilesetsChanged(value);
}

void AppState::setProjectScreenshots(QList<Screenshot*> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectScreenshots, value);
    _projectScreenshots = value;
    recreateIndex(value, _index_Screenshot_ID, [](Screenshot * item){ return item->id; });
    emit onProjectScreenshotsChanged(value);
}

Tile * AppState::getProjectTileById(int id)
{
    auto it = _index_Tile_ID.find(id);
    return it == _index_Tile_ID.end() ? nullptr : it.value();
}

Palette * AppState::getProjectPaletteById(int id)
{
    auto it = _index_Palette_ID.find(id);
    return it == _index_Palette_ID.end() ? nullptr : it.value();
}

Tileset * AppState::getProjectTilesetById(int id)
{
    auto it = _index_Tileset_ID.find(id);
    return it == _index_Tileset_ID.end() ? nullptr : it.value();
}

Reference * AppState::getProjectReferenceById(int id)
{
    auto it = _index_Reference_ID.find(id);
    return it == _index_Reference_ID.end() ? nullptr : it.value();
}

Screenshot *AppState::getProjectScreenshotById(int id)
{
    auto it = _index_Screenshot_ID.find(id);
    return it == _index_Screenshot_ID.end() ? nullptr : it.value();
}

QList<Reference *> AppState::getProjectReferencesByTileId(int tileId)
{
    QList<Reference*> result;
    if (_projectReferences != nullptr)
        for (auto r : *_projectReferences)
            if (r->tileId == tileId)
                result.append(r);
    return result;

}

void AppState::appendProjectTile(Tile *value)
{
    _projectTiles->append(value);
    _index_Tile_ID[value->id] = value;
}

void AppState::appendProjectPalette(Palette *value)
{
    _projectPalettes->append(value);
    _index_Palette_ID[value->id] = value;
}

void AppState::appendProjectTileset(Tileset *value)
{
    _projectTilesets->append(value);
    _index_Tileset_ID[value->id] = value;
}

void AppState::appendProjectReference(Reference *value)
{
    _projectReferences->append(value);
    _index_Reference_ID[value->id] = value;
}

void AppState::appendProjectScreenshot(Screenshot *value)
{
    _projectScreenshots->append(value);
    _index_Screenshot_ID[value->id] = value;
}

void AppState::addProjectTileset(int const position, Tileset * value)
{
    if (value == nullptr)
        return;

    _projectTilesets->insert(position, value);
    _index_Tileset_ID[value->id] = value;
    emit onProjectTilesetsInserted(_projectTilesets, position);
}

void AppState::dropProjectTileset(int const position)
{
    if (position < 0 || position >= _projectTilesets->size())
        return;

    auto ts = _projectTilesets->at(position);
    _projectTilesets->remove(position);
    _index_Tileset_ID.remove(ts->id);
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

bool AppState::projectHasChanges()
{
    return _projectHasChanges;
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

void AppState::setTilesFilter(TilesFilter * value)
{
    if (_tilesFilter != nullptr && value != _tilesFilter)
        delete _tilesFilter;

    _tilesFilter = value;
    emit onTilesFilterChanged(value);
}

TilesFilter * AppState::tilesFilter() const
{
    return _tilesFilter;
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

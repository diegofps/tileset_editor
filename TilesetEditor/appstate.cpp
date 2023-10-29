#include "appstate.h"
#include "editorcommand.h"

AppState::AppState()
{
    _selectedTileset = nullptr;

    _palettesMode = USED_BY_TILE;
    _selectedPalette = nullptr;

    _referenceMode = REF_25;
    _referenceZoom = 17;
    _referenceOffsetImage = nullptr;
    _referenceOffset = QPoint(0,0);

    _editorTool = PENCIL;
    _editorShowLinkedTiles = true;
    _editorShowUnlinkedTiles = true;
    _editorShowGrid = true;
    _editorRoot = QPoint(0,0);
    _editorZoom = 15;

    _project = nullptr;
    _projectHasChanges = false;
    _projectTiles = nullptr;
    _projectPalettes = nullptr;
    _projectTilesets = nullptr;
    _projectReferences = nullptr;
    _projectScreenshots = nullptr;
    _projectScenes = nullptr;
    _selectedSceneID = 0;
    _lastMoveToSceneResult = 0;

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
    if (value != _lastDumpFolder)
    {
        _lastDumpFolder = value;
        emit onLastDumpFolderChanged(value);
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

void AppState::setAllTiles(QList<Tile *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTiles, value);
    _projectTiles = value;
    recreateIndex(value, _index_Tile_ID, [](Tile* item){ return item->id; });
    emit onAllTilesChanged(value);
    updateFilteredTiles();
}

void AppState::setAllPalettes(QList<Palette *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectPalettes, value);
    _projectPalettes = value;
    recreateIndex(value, _index_Palette_ID, [](Palette* item){ return item->id; });
    emit onAllPalettesChanged(value);
    updateFilteredPalettes();
}

void AppState::setAllReferences(QList<Reference *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectReferences, value);
    _projectReferences = value;
    recreateIndex(value, _index_Reference_ID, [](Reference * item){ return item->id; });
    emit onAllReferencesChanged(value);
}

void AppState::setAllTilesets(QList<Tileset *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectTilesets, value);
    _projectTilesets = value;
    recreateIndex(value, _index_Tileset_ID, [](Tileset* item){ return item->id; });
    emit onAllTilesetsChanged(value);
    updateFilteredTilesets();
}

void AppState::setAllScreenshots(QList<Screenshot*> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectScreenshots, value);
    _projectScreenshots = value;
    recreateIndex(value, _index_Screenshot_ID, [](Screenshot * item){ return item->id; });
    emit onAllScreenshotsChanged(value);
}

void AppState::setAllScenes(QList<Scene *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectScenes, value);
    _projectScenes = value;
    recreateIndex(value, _index_Scene_ID, [](Scene * item){ return item->id; });
    emit onAllScenesChanged(value);
}

void AppState::setSelectedSceneID(int sceneID)
{
    if (_selectedSceneID != sceneID)
    {
        _selectedSceneID = sceneID;
        emit onSelectedSceneIDChanged(sceneID);
        updateFilteredTilesets();
        updateFilteredTiles();
    }
}

Tile * AppState::getTileById(int id)
{
    auto it = _index_Tile_ID.find(id);
    return it == _index_Tile_ID.end() ? nullptr : it.value();
}

Palette * AppState::getPaletteById(int id)
{
    auto it = _index_Palette_ID.find(id);
    return it == _index_Palette_ID.end() ? nullptr : it.value();
}

Tileset * AppState::getTilesetById(int id)
{
    auto it = _index_Tileset_ID.find(id);
    return it == _index_Tileset_ID.end() ? nullptr : it.value();
}

Reference * AppState::getReferenceById(int id)
{
    auto it = _index_Reference_ID.find(id);
    return it == _index_Reference_ID.end() ? nullptr : it.value();
}

Screenshot *AppState::getScreenshotById(int id)
{
    auto it = _index_Screenshot_ID.find(id);
    return it == _index_Screenshot_ID.end() ? nullptr : it.value();
}

Scene *AppState::getSceneById(int id)
{
    auto it = _index_Scene_ID.find(id);
    return it == _index_Scene_ID.end() ? nullptr : it.value();
}

const QList<Tile *> *AppState::filteredTiles() const
{
    return &_filteredTiles;
}

const QList<Palette *> *AppState::filteredPalettes() const
{
    return &_filteredPalettes;
}

const QList<Tileset *> *AppState::filteredTilesets() const
{
    return &_filteredTilesets;
}

void AppState::updateFilteredTiles()
{
    _filteredTiles.clear();

    if (_projectTiles == nullptr)
    {
        emit onFilteredTilesChanged(&_filteredTiles);
        return;
    }

    for (auto t : *_projectTiles)
    {
        if ( _selectedSceneID!=-1)
            if (t->sceneId!=_selectedSceneID)
                continue;

        if (_tilesFilter.isUnlinked!=2)
            if ((_tilesFilter.isUnlinked==0 && t->linkedCellID!=0) || (_tilesFilter.isUnlinked==1 && t->linkedCellID==0))
                continue;

        if (_tilesFilter.usedInSprite!=2)
            if ((_tilesFilter.usedInSprite==0 && t->usedInSprite) || (_tilesFilter.usedInSprite==1 && !t->usedInSprite))
                continue;

        if (_tilesFilter.usedInBackground!=2)
            if ((_tilesFilter.usedInBackground==0 && t->usedInBackground) || (_tilesFilter.usedInBackground==1 && !t->usedInBackground))
                continue;

        if (_tilesFilter.usedWithHFlip!=2)
            if ((_tilesFilter.usedWithHFlip==0 && t->usedWithHFlip()) || (_tilesFilter.usedWithHFlip==1 && !t->usedWithHFlip()))
                continue;

        if (_tilesFilter.usedWithVFlip!=2)
            if ((_tilesFilter.usedWithVFlip==0 && t->usedWithVFlip()) || (_tilesFilter.usedWithVFlip==1 && !t->usedWithVFlip()))
                continue;

        _filteredTiles.append(t);
    }

    emit onFilteredTilesChanged(&_filteredTiles);
    updateFilteredPalettes();
}

void AppState::updateFilteredTilesets()
{
    _filteredTilesets.clear();

    if (_projectTilesets == nullptr)
    {
        emit onFilteredTilesetsChanged(&_filteredTilesets);
        return;
    }

    for (auto ts : *_projectTilesets)
    {
        if (_selectedSceneID != -1)
            if (_selectedSceneID != ts->sceneId)
                continue;

        _filteredTilesets.append(ts);
    }

    emit onFilteredTilesetsChanged(&_filteredTilesets);
}

void AppState::updateFilteredPalettes()
{
    _filteredPalettes.clear();

    if (_projectPalettes == nullptr)
    {
        emit onFilteredPalettesChanged(&_filteredPalettes);
        return;
    }

    if (_palettesMode == ALL)
    {
        for (auto item : *_projectPalettes)
            _filteredPalettes.append(item);
        qDebug() << "Filter found " << _filteredPalettes.size() << " palettes using all";
        emit onFilteredPalettesChanged(&_filteredPalettes);
        return;
    }

    if (_selectedTiles.isEmpty())
        return;

    for (auto palette : *_projectPalettes)
    {
        bool add = false;

        for (auto tile : _selectedTiles)
        {
            if (tile != nullptr && tile->palettesUsed.contains(palette->id))
            {
                add = true;
                break;
            }
        }

        if (add)
            _filteredPalettes.append(palette);
    }

    qDebug() << "Filter found " << _filteredPalettes.size() << " palettes not using all";
    emit onFilteredPalettesChanged(&_filteredPalettes);
}

void AppState::addTile(Tile *value)
{
    _projectTiles->append(value);
    _index_Tile_ID[value->id] = value;
}

void AppState::addPalette(Palette *value)
{
    _projectPalettes->append(value);
    _index_Palette_ID[value->id] = value;
}

void AppState::addTileset(Tileset *value)
{
    _projectTilesets->append(value);
    _index_Tileset_ID[value->id] = value;
}

void AppState::addReference(Reference *value)
{
    _projectReferences->append(value);
    _index_Reference_ID[value->id] = value;
}

void AppState::addScreenshot(Screenshot *value)
{
    _projectScreenshots->append(value);
    _index_Screenshot_ID[value->id] = value;
}

void AppState::addScene(Scene *value)
{
    _projectScenes->append(value);
    _index_Scene_ID[value->id] = value;
}

void AppState::insertScene(const int position, Scene *value)
{
    if (value == nullptr)
        return;

    _projectScenes->insert(position, value);
    _index_Scene_ID[value->id] = value;

    emit onSceneInserted(_projectScenes, position);
}

void AppState::removeScene(const int position)
{
    if (position < 0 || position >= _projectScenes->size())
        return;

    auto value = _projectScenes->at(position);
    _projectScenes->remove(position);
    _index_Scene_ID.remove(value->id);

    for (auto t : *_projectTiles)
        if (t->sceneId == value->id)
            t->sceneId = 0;

    for (auto t : *_projectTilesets)
        if (t->sceneId == value->id)
            t->sceneId = 0;

    emit onSceneRemoved(_projectScenes, position);
    delete value;
}

void AppState::moveUpScene(const int position)
{
    if (position < 0 || position >= _projectScenes->size()-1)
        return;

    auto ts = _projectScenes->takeAt(position);
    _projectScenes->insert(position+1, ts);

    emit onSceneMoved(_projectScenes, position, position+1);
}

void AppState::moveDownScene(const int position)
{
    if (position <= 0 || position >= _projectScenes->size())
        return;

    auto ts = _projectScenes->takeAt(position);
    _projectScenes->insert(position-1, ts);

    emit onSceneMoved(_projectScenes, position, position-1);
}

void AppState::insertTileset(int const position, Tileset * value)
{
    if (value == nullptr)
        return;

    _projectTilesets->insert(position, value);
    _index_Tileset_ID[value->id] = value;
    _projectHasChanges = true;

    emit onTilesetInserted(_projectTilesets, position);
}

void AppState::removeTileset(int const position)
{
    if (position < 0 || position >= _projectTilesets->size())
        return;

    auto ts = _projectTilesets->at(position);
    _projectTilesets->remove(position);
    _index_Tileset_ID.remove(ts->id);
    _projectHasChanges = true;

    emit onTilesetRemoved(_projectTilesets, position);
    delete ts;
}

void AppState::moveUpTileset(int const position)
{
    if (position < 0 || position >= _projectTilesets->size()-1)
        return;

    auto ts = _projectTilesets->takeAt(position);
    _projectTilesets->insert(position+1, ts);
    _projectHasChanges = true;

    emit onTilesetMoved(_projectTilesets, position, position+1);
}

void AppState::moveDownTileset(int const position)
{
    if (position <= 0 || position >= _projectTilesets->size())
        return;

    auto ts = _projectTilesets->takeAt(position);
    _projectTilesets->insert(position-1, ts);
    _projectHasChanges = true;

    emit onTilesetMoved(_projectTilesets, position, position-1);
}

void AppState::setLastMoveToSceneResult(int const value)
{
    _lastMoveToSceneResult = value;
}

int AppState::lastMoveToSceneResult()
{
    return _lastMoveToSceneResult;
}

int AppState::selectedSceneID()
{
    return _selectedSceneID;
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
    return _lastDumpFolder;
}

QList<Tile *> * AppState::allTiles() const
{
    return _projectTiles;
}

QList<Palette *> * AppState::allPalettes() const
{
    return _projectPalettes;
}

QList<Reference *> * AppState::allReferences() const
{
    return _projectReferences;
}

QList<Screenshot *> * AppState::allScreenshots() const
{
    return _projectScreenshots;
}

QList<Scene *> *AppState::allScenes() const
{
    return _projectScenes;
}

//int AppState::scenePos2ID(int scenePos)
//{
//    if (scenePos < 2)
//        return scenePos - 1;

//    scenePos -= 2;

//    if (scenePos >= _projectScenes->size())
//        return 0;

//    else
//        return _projectScenes->at(scenePos)->id;
//}

QList<Tileset *> * AppState::allTilesets() const
{
    return _projectTilesets;
}


// Editor Toolbox

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

QPoint AppState::editorRoot()
{
    return _editorRoot;
}

void AppState::moveEditorRoot(int rx, int ry)
{
    _editorRoot.setX(_editorRoot.x() + rx);
    _editorRoot.setY(_editorRoot.y() + ry);
    emit onEditorRootChanged(_editorRoot);
}

void AppState::moveEditorRootHome()
{
    _editorRoot.setX(0);
    _editorRoot.setY(0);
    emit onEditorRootChanged(_editorRoot);
}

void AppState::zoomInEditor()
{
    if (_editorZoom > 13)
    {
        --_editorZoom;
        emit onEditorZoomChanged(_editorZoom);
    }
}

void AppState::zoomOutEditor()
{
    if (_editorZoom < 20)
    {
        ++_editorZoom;
        emit onEditorZoomChanged(_editorZoom);
    }
}

int AppState::editorZoom()
{
    return _editorZoom;
}

void AppState::moveCellsInTileset(int rx, int ry)
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    QList<Cell*> cells;

    for (auto pair : tileset->cells.asKeyValueRange())
    {
        pair.second->x += rx;
        pair.second->y += ry;
        cells.append(pair.second);
    }

    tileset->cells.clear();

    for (auto cell : cells)
    {
        QPair<int,int> key(cell->x, cell->y);
        tileset->cells.insert(key, cell);
    }

    emit onSelectedTilesetChanged(tileset);
}

void AppState::moveViewport(int rx, int ry)
{
    emit onMoveViewport(rx, ry);
}

void AppState::moveViewportHome()
{
    emit onMoveViewportHome();
}

void AppState::editorPaintCellUsingSibling()
{

}

void AppState::editorPaintCellUsingSelection(int x, int y)
{
    auto  project = _project;
    auto  palette = _selectedPalette;
    auto  tileset = _selectedTileset;
    auto  tile = selectedTile();
    auto& tilePreviewFilter = _tilePreviewFilter;

    if (project == nullptr || palette == nullptr || tile == nullptr || tileset == nullptr)
        return;

    Cell cell;
    cell.id = ++_project->lastCellID;
    cell.hFlip = tilePreviewFilter.hFlip;
    cell.vFlip = tilePreviewFilter.vFlip;
    cell.paletteID = palette->id;
    cell.tileID = tile->id;
    cell.x = x;
    cell.y = y;

    QPair<int,int> position(x,y);
    auto it = tileset->cells.constFind(position);
    auto previous = it == tileset->cells.constEnd() ? nullptr : it.value();
    bool wasLinked = false;

    if (previous != nullptr)
    {
        auto previousTile = getTileById(previous->tileID);
        if (previousTile != nullptr && previousTile->linkedCellID == previous->id)
            wasLinked = true;
    }

    tileset->historyAdd(this, new PaintCommand(position, previous, &cell, wasLinked));
    emit onSelectedTilesetChanged(tileset);
}

void AppState::editorEraseCell(int x, int y)
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    QPair<int,int> position(x,y);
    auto it = tileset->cells.constFind(position);

    if (it == tileset->cells.constEnd())
        return;

    auto previous = it == tileset->cells.constEnd() ? nullptr : it.value();
    bool wasLinked = false;

    if (previous != nullptr)
    {
        auto previousTile = getTileById(previous->tileID);
        if (previousTile != nullptr && previousTile->linkedCellID == previous->id)
            wasLinked = true;
    }

    tileset->historyAdd(this, new PaintCommand(position, it.value(), nullptr, wasLinked));
    emit onSelectedTilesetChanged(tileset);
}

void AppState::editorColorPickCell(int x, int y)
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    QPair<int,int> position(x,y);
    auto it = tileset->cells.constFind(position);

    if (it == tileset->cells.constEnd())
        return;

    auto tile = getTileById(it.value()->tileID);
    auto palette = getPaletteById(it.value()->paletteID);

    QList<Tile*> selectedTiles;
    selectedTiles.append(tile);
    setSelectedTiles(selectedTiles);
    setSelectedPalette(palette);
}

void AppState::editorToggleCellIsLink(int x, int y)
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    QPair<int,int> position(x,y);
    auto it = tileset->cells.constFind(position);

    if (it == tileset->cells.constEnd())
        return;

    auto & cell = it.value();
    auto tile = getTileById(cell->tileID);

    if (tile == nullptr)
        return;

    int const oldCellID = tile->linkedCellID;
    int const newCellID = tile->linkedCellID==cell->id ? 0 : cell->id;

    tileset->historyAdd(this, new LinkCommand(position, oldCellID, newCellID));
    emit onSelectedTilesetChanged(tileset);
}

void AppState::editorUndo()
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    if (tileset->historyUndo(this))
        emit onSelectedTilesetChanged(tileset);
}

void AppState::editorRedo()
{
    auto tileset = _selectedTileset;

    if (tileset == nullptr)
        return;

    if (tileset->historyRedo(this))
        emit onSelectedTilesetChanged(tileset);
}

void AppState::notifyCellCreated(Cell const *)
{

}

void AppState::notifyCellDeleted(Cell const * cell)
{
    auto tile = getTileById(cell->tileID);

    if (tile == nullptr || tile->linkedCellID != cell->id)
        return;

    tile->linkedCellID = 0;
}

// References Toolbox

ReferenceMode AppState::referenceMode() const
{
    return _referenceMode;
}

QImage * AppState::referenceOffsetImage()
{
    return _referenceOffsetImage;
}

int AppState::referenceZoom()
{
    return _referenceZoom;
}

void AppState::setReferenceMode(ReferenceMode value)
{
    if (value != _referenceMode)
    {
        _referenceMode = value;
        emit onReferenceModeChanged(value);
    }
}

void AppState::setReferenceOffsetImage(QImage * value)
{
    _referenceOffsetImage = value;
    emit onReferenceOffsetImageChanged(value);
}

void AppState::zoomInReference()
{
    if (_referenceZoom > 13)
    {
        --_referenceZoom;
        emit onReferenceZoomChanged(_referenceZoom);
    }
}

void AppState::zoomOutReference()
{
    if (_referenceZoom < 20)
    {
        ++_referenceZoom;
        emit onReferenceZoomChanged(_referenceZoom);
    }
}

QPoint AppState::referenceOffset()
{
    return _referenceOffset;
}

void AppState::moveReferenceOffset(int rx, int ry)
{
    _referenceOffset.setX(_referenceOffset.x() + rx);
    _referenceOffset.setY(_referenceOffset.y() + ry);
    emit onReferenceOffsetChanged(_referenceOffset);
}

void AppState::moveReferenceOffsetHome()
{
    _referenceOffset.setX(0);
    _referenceOffset.setY(0);
    emit onReferenceOffsetChanged(_referenceOffset);
}

// Tiles

void AppState::setTilesFilter(TilesFilter const & value)
{
    if (_tilesFilter != value)
    {
        _tilesFilter = value;
        emit onTilesFilterChanged(value);
        updateFilteredTiles();
    }
}

void AppState::setSelectedTiles(QList<Tile*> & tiles)
{
    _selectedTiles = tiles;
    emit onSelectedTilesChanged(&_selectedTiles);
    updateFilteredPalettes();
}

TilesFilter const & AppState::tilesFilter() const
{
    return _tilesFilter;
}

QList<Tile*> const * AppState::selectedTiles() const
{
    return &_selectedTiles;
}

Tile * AppState::selectedTile() const
{
    return _selectedTiles.isEmpty() ? nullptr : _selectedTiles[0];
}

void AppState::moveSelectedTilesToScene(int sceneID)
{
    auto tiles = _selectedTiles;

    if (sceneID == -1)
    {
        for (auto tile : tiles)
            tile->sceneId = 0;
    }
    else
    {
        for (auto tile : tiles)
            tile->sceneId = sceneID;
    }

    _projectHasChanges = true;

    emit onAllTilesChanged(_projectTiles);
    updateFilteredTiles();
}

void AppState::moveTileSelection(int rx, int ry)
{
    emit onMoveTileSelection(rx, ry);
}

// Palettes

void AppState::setPalettesMode(PaletteMode value)
{
    if (value != _palettesMode)
    {
        _palettesMode = value;
        emit onPalettesModeChanged(value);
        updateFilteredPalettes();
    }
}

void AppState::setSelectedPalette(Palette * value)
{
    _selectedPalette = value;
    emit onSelectedPaletteChanged(value);
}

TilePreviewFilter & AppState::tilePreviewFilter()
{
    return _tilePreviewFilter;
}

PaletteMode AppState::palettesMode() const
{
    return _palettesMode;
}

Palette * AppState::selectedPalette()
{
    return _selectedPalette;
}

// TilePreview

void AppState::setTilePreviewFilter(TilePreviewFilter const & value)
{
    if (value != _tilePreviewFilter)
    {
        _tilePreviewFilter = value;
        emit onTileFilterChanged(value);
    }
}

Tileset * AppState::selectedTileset() const
{
    return _selectedTileset;
}

// Tilesets

void AppState::setSelectedTileset(Tileset * value)
{
    _selectedTileset = value;
    emit onSelectedTilesetChanged(value);
}

void AppState::moveSelectedTilesetToScene(int sceneID)
{
    if (_selectedTileset == nullptr)
        return;

    _selectedTileset->sceneId = sceneID==-1?0:sceneID;
    _projectHasChanges = true;

    emit onAllTilesetsChanged(_projectTilesets);
    updateFilteredTilesets();
}

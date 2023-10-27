#include "appstate.h"

AppState::AppState()
{
    _selectedTileset = nullptr;

    _palettesMode = USED_BY_TILE;
    _selectedPalette = nullptr;

    _referenceMode = REF_1;
    _referenceZoom = 15;
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
    _selectedScenePos = 0;
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

void AppState::setProjectScenes(QList<Scene *> *value)
{
    deleteItemsAndQListIfNotNullptr(_projectScenes, value);
    _projectScenes = value;
    recreateIndex(value, _index_Scene_ID, [](Scene * item){ return item->id; });
    emit onProjectScenesChanged(value);
}

void AppState::setSelectedScenePos(int scenePos)
{
    _selectedScenePos = scenePos;
    emit onSelectedScenePosChanged(scenePos);
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

Scene *AppState::getProjectSceneById(int id)
{
    auto it = _index_Scene_ID.find(id);
    return it == _index_Scene_ID.end() ? nullptr : it.value();
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

void AppState::appendProjectScene(Scene *value)
{
    _projectScenes->append(value);
    _index_Scene_ID[value->id] = value;
}

void AppState::insertProjectScene(const int position, Scene *value)
{
    if (value == nullptr)
        return;

    _projectScenes->insert(position, value);
    _index_Scene_ID[value->id] = value;

    emit onProjectScenesInserted(_projectScenes, position);
}

void AppState::removeProjectScene(const int position)
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

    emit onProjectScenesRemoved(_projectScenes, position);
    delete value;
}

void AppState::moveUpProjectScene(const int position)
{
    if (position < 0 || position >= _projectScenes->size()-1)
        return;

    auto ts = _projectScenes->takeAt(position);
    _projectScenes->insert(position+1, ts);

    emit onProjectScenesMoved(_projectScenes, position, position+1);
}

void AppState::moveDownProjectScene(const int position)
{
    if (position <= 0 || position >= _projectScenes->size())
        return;

    auto ts = _projectScenes->takeAt(position);
    _projectScenes->insert(position-1, ts);

    emit onProjectScenesMoved(_projectScenes, position, position-1);
}

void AppState::insertProjectTileset(int const position, Tileset * value)
{
    if (value == nullptr)
        return;

    _projectTilesets->insert(position, value);
    _index_Tileset_ID[value->id] = value;

    emit onProjectTilesetsInserted(_projectTilesets, position);
}

void AppState::removeProjectTileset(int const position)
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

void AppState::setLastMoveToSceneResult(int const value)
{
    _lastMoveToSceneResult = value;
}

int AppState::lastMoveToSceneResult()
{
    return _lastMoveToSceneResult;
}

int AppState::selectedScenePos()
{
    return _selectedScenePos;
}

//void AppState::drawNearestReferenceTile()
//{
//    // TODO
//}

//void AppState::drawTile()
//{
//    // TODO
//}

//void AppState::eraseTile()
//{
//    // TODO
//}

//void AppState::linkTile()
//{
//    // TODO
//}

//void AppState::undo()
//{
//    // TODO
//}

//void AppState::redo()
//{
//    // TODO
//}

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

QList<Scene *> *AppState::projectScenes() const
{
    return _projectScenes;
}

int AppState::scenePos2ID(int scenePos)
{
    if (scenePos < 2)
        return scenePos - 1;

    scenePos -= 2;

    if (scenePos >= _projectScenes->size())
        return 0;

    else
        return _projectScenes->at(scenePos)->id;
}

QList<Tileset *> * AppState::projectTilesets() const
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

}

void AppState::editorEraseCell(int x, int y)
{

}

void AppState::editorColorPickCell(int x, int y)
{

}

void AppState::editorLinkCell(int x, int y)
{

}

void AppState::editorUndo()
{

}

void AppState::editorRedo()
{

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
    _tilesFilter = value;
    emit onTilesFilterChanged(value);
}

void AppState::setSelectedTilesPos(Range value)
{
    _selectedTilesPos = value;
    emit onSelectedTilesPosChanged(_selectedTilesPos);
}

TilesFilter const & AppState::tilesFilter() const
{
    return _tilesFilter;
}

Range const * AppState::selectedTilesPos() const
{
    return & _selectedTilesPos;
}

int AppState::selectedTilePos() const
{
    return _selectedTilesPos.isEmpty() ? 0 : _selectedTilesPos.start;
}

QList<Tile *> AppState::selectedTiles() const
{
    QList<Tile*> tiles;
    Range tmp = _selectedTilesPos;
    tmp.limit(0, _projectTiles->size());

    if (!tmp.isEmpty())
        for (qsizetype tilePos=tmp.start;tilePos<=tmp.end;++tilePos)
            tiles.append(_projectTiles->at(tilePos));

    return tiles;
}

Tile * AppState::selectedTile() const
{
    Range range = _selectedTilesPos;
    range.limit(0, _projectTiles->size());

    if (range.isEmpty())
        return nullptr;

    return _projectTiles->at(range.start);
}

void AppState::moveSelectedTilesToScene(int sceneID)
{
    Range range = _selectedTilesPos;
    range.limit(0, _projectTiles->size());

    if (range.isEmpty())
        return;

    if (sceneID == -1)
    {
        for (qsizetype pos=range.start;pos!=range.end;++pos)
            _projectTiles->at(pos)->sceneId = 0;
    }
    else
    {
        for (qsizetype pos=range.start;pos!=range.end;++pos)
            _projectTiles->at(pos)->sceneId = sceneID;
    }

    _projectHasChanges = true;

    emit onProjectTilesChanged(_projectTiles);
}

void AppState::selectTileAtEditorPosition(int rx, int ry)
{
    emit onSelectTileAtEditorPosition(rx, ry);
}

// Palettes

void AppState::setPalettesMode(PaletteMode value)
{
    if (value != _palettesMode)
    {
        _palettesMode = value;
        emit onPalettesModeChanged(value);
    }
}

void AppState::setSelectedPalette(Palette * value)
{
    _selectedPalette = value;
    emit onSelectedPaletteChanged(value);
}

TileMode & AppState::tileMode()
{
    return _tileMode;
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

void AppState::setTileMode(TileMode const & value)
{
    if (value != _tileMode)
    {
        _tileMode = value;
        emit onTileModeChanged(value);
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

    emit onProjectTilesetsChanged(_projectTilesets);
}

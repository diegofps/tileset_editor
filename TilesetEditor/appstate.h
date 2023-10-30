#ifndef APPSTATE_H
#define APPSTATE_H

#include "model/scene.h"
#include "model/project.h"
#include "model/reference.h"
#include "model/screenshot.h"
#include "model/tile.h"
#include "model/palette.h"
#include "model/tileset.h"

#include "range.h"

#include <QObject>
#include <string>
#include <QPoint>

//enum EditorTool
//{
//    PENCIL,
//    ERASER,
//    LINKER
//};

enum ReferenceMode
{
    REF_1,
    REF_25,
    REF_50,
    REF_75,
    REF_100,
    REF_NN,
    REF_NF,
    REF_FN,
    REF_FF
};

enum PaletteMode
{
    ALL,
    USED_BY_TILE
};

class TilesFilter
{
public:

    int usedInSprite;
    int usedInBackground;
    int isUnlinked;
    int usedWithHFlip;
    int usedWithVFlip;

    TilesFilter() :
        usedInSprite(2),
        usedInBackground(2),
        isUnlinked(2),
        usedWithHFlip(2),
        usedWithVFlip(2)
    { }

    bool operator!=(TilesFilter const & other) const
    {
        return ! (*this == other);
    }

    bool operator==(TilesFilter const & other) const
    {
        return usedInSprite == other.usedInSprite &&
                usedInBackground == other.usedInBackground &&
                isUnlinked == other.isUnlinked &&
                usedWithHFlip == other.usedWithHFlip &&
                usedWithVFlip == other.usedWithVFlip;
    }
};

class TilePreviewFilter
{
public:

    bool hFlip;
    bool vFlip;

public:

    TilePreviewFilter() : hFlip(false), vFlip(false) {}

    bool operator!=(TilePreviewFilter const & other) const
    {
        return !(*this == other);
    }

    bool operator==(TilePreviewFilter const & other) const
    {
        return hFlip == other.hFlip && vFlip == other.vFlip;
    }
};


class AppState : public QObject
{
private:

    Q_OBJECT

    // Context Folder
    Project            * _project;
    bool                 _projectHasChanges;
    QString              _lastDumpFolder;
    QList<Tile*>       * _projectTiles;
    QList<Palette*>    * _projectPalettes;
    QList<Tileset*>    * _projectTilesets;
    QList<Reference*>  * _projectReferences;
    QList<Screenshot*> * _projectScreenshots;
    QList<Scene*>      * _projectScenes;
    int                  _selectedSceneID;
    int                  _lastMoveToSceneResult;
    bool                 _showLinkInfo;

    QList<Tile*>    _filteredTiles;
    QList<Palette*> _filteredPalettes;
    QList<Tileset*> _filteredTilesets;

    QHash<int, Tile*>       _index_Tile_ID;
    QHash<int, Tileset*>    _index_Tileset_ID;
    QHash<int, Screenshot*> _index_Screenshot_ID;
    QHash<int, Palette*>    _index_Palette_ID;
    QHash<int, Reference*>  _index_Reference_ID;
    QHash<int, Scene*>      _index_Scene_ID;


    // Editor Toolbox
//    EditorTool _editorTool;
//    bool       _editorShowLinkedTiles;
//    bool       _editorShowUnlinkedTiles;
//    bool       _editorShowGrid;
    QPoint     _editorRoot;
    int        _editorZoom;

    // References Toolbox
    ReferenceMode   _referenceMode;
    int             _referenceZoom;
    QImage        * _referenceOffsetImage;
    QPoint          _referenceOffset;

    // Tiles
    TilesFilter  _tilesFilter;
    QList<Tile*> _selectedTiles;

    // Palettes
    PaletteMode   _palettesMode;
    Palette     * _selectedPalette;

    // Tile Preview
    TilePreviewFilter _tilePreviewFilter;

    // Tilesets
    Tileset * _selectedTileset;

public:

    AppState();

    // Context Folder
    Project * project() const;
    bool projectHasChanges();
    QString const & projectLastDumpFolder() const;

    void setProject(Project * value);
    void setProjectHasChanges(bool value);
    void setProjectLastDumpFolder(QString value);

    QList<Tile *>       * allTiles() const;
    QList<Palette *>    * allPalettes() const;
    QList<Tileset *>    * allTilesets() const;
    QList<Reference *>  * allReferences() const;
    QList<Screenshot *> * allScreenshots() const;
    QList<Scene *>      * allScenes() const;

    void setAllTiles(QList<Tile *> * value);
    void setAllPalettes(QList<Palette *> * value);
    void setAllTilesets(QList<Tileset *> * value);
    void setAllReferences(QList<Reference *> * value);
    void setAllScreenshots(QList<Screenshot *> * value);
    void setAllScenes(QList<Scene*> * value);

    Tile       * getTileById(int id);
    Palette    * getPaletteById(int id);
    Tileset    * getTilesetById(int id);
    Reference  * getReferenceById(int id);
    Screenshot * getScreenshotById(int id);
    Scene      * getSceneById(int id);

    QList<Tile *>    const * filteredTiles() const;
    QList<Palette *> const * filteredPalettes() const;
    QList<Tileset *> const * filteredTilesets() const;

    void updateFilteredTiles();
    void updateFilteredTilesets();
    void updateFilteredPalettes();

    void addTile(Tile * value);
    void addPalette(Palette * value);
    void addTileset(Tileset * value);
    void addReference(Reference * value);
    void addScreenshot(Screenshot * value);
    void addScene(Scene * value);

    void insertScene(int const position, Scene * value);
    void removeScene(int const position);
    void moveUpScene(int const position);
    void moveDownScene(int const position);

    void insertTileset(int const position, Tileset * value);
    void removeTileset(int const position);
    void moveUpTileset(int const position);
    void moveDownTileset(int const position);

    void setLastMoveToSceneResult(int const value);
    int  lastMoveToSceneResult();

    // Scene
    int selectedSceneID();

    void setShowLinkInfo(bool value);
    bool showLinkInfo();

    // Editor Toolbox
//    EditorTool editorTool() const;
//    bool editorShowLinkedTiles() const;
//    bool editorShowUnlinkedTiles() const;
//    bool editorShowGrid() const;

//    void setEditorTool(EditorTool const value);
//    void setEditorShowLinkedTiles(bool value);
//    void setEditorShowUnlinkedTiles(bool value);
//    void setEditorShowGrid(bool value);

    QPoint editorRoot();
    void moveEditorRoot(int rx, int ry);
    void moveEditorRootHome();

    void zoomInEditor();
    void zoomOutEditor();
    int  editorZoom();

    void moveCellsInTileset(int rx, int ry);
    void moveViewport(int rx, int ry);
    void moveViewportHome();

    void editorPaintCellUsingSibling();
    void editorPaintCellUsingSelection();
    void editorPaintCellUsingSelection(int x, int y, bool paintTile, bool paintPalette);
    void editorPaintCell(int x, int y, Tile * tile, Palette * palette, bool hFlip, bool vFlip);
    void editorEraseCell(int x, int y);
    void editorColorPickCell(int x, int y);
    void editorToggleCellIsLink(int x, int y);
    void editorUndo();
    void editorRedo();
    void autoLink();
    void autoUnlink();
    void clearCell();
    void flushCellsWithSelectedPalettes();

    void notifyCellCreated(Cell const * cell);
    void notifyCellDeleted(Cell const * cell, int moveLinkedCellID);


    // References Toolbox
    ReferenceMode referenceMode() const;
    QImage*       referenceOffsetImage();
    int           referenceZoom();

    void setReferenceMode(ReferenceMode value);
    void setReferenceHighlightPosition(bool value);
    void setReferenceOffsetImage(QImage * offsetImage);

    void zoomInReference();
    void zoomOutReference();

    QPoint referenceOffset();
    void   moveReferenceOffset(int rx, int ry);
    void   moveReferenceOffsetHome();

    // Tiles
    TilesFilter const&  tilesFilter() const;
    QList<Tile*> const* selectedTiles() const;
    Tile*               selectedTile() const;
    void                moveSelectedTilesToScene(int sceneID);
    void                moveTileSelection(int rx, int ry);

    // Palettes
    PaletteMode palettesMode() const;
    Palette*    selectedPalette();
    void        setPalettesMode(PaletteMode value);
    void        setSelectedPalette(Palette * value);

    // Tile Preview
    TilePreviewFilter& tilePreviewFilter();
    void      setTilePreviewFilter(TilePreviewFilter const & value);

    // Tilesets
    Tileset* selectedTileset() const;
    void     setSelectedTileset(Tileset * value);
    void     moveSelectedTilesetToScene(int sceneID);

public:

    // Scenes
    void setSelectedSceneID(int sceneID);

    //Tiles
    void setTilesFilter(TilesFilter const & value);
    void setSelectedTiles(QList<Tile*> & value);

signals:

    // Context Folder
    void onProjectChanged(Project * value);
    void onProjectHasChangesChanged(bool value);
    void onLastDumpFolderChanged(QString const & value);
    void onAllTilesChanged(QList<Tile *> const * value);
    void onAllPalettesChanged(QList<Palette *> const * value);
    void onAllTilesetsChanged(QList<Tileset *> const * value);
    void onAllReferencesChanged(QList<Reference *> const * value);
    void onAllScreenshotsChanged(QList<Screenshot*> const * value);
    void onAllScenesChanged(QList<Scene*> const * value);

    void onSceneInserted(QList<Scene *> const * value, int const position);
    void onSceneRemoved(QList<Scene *> const * value, int const position);
    void onSceneMoved(QList<Scene *> const * value, int const oldPosition, int const newPosition);

    void onTilesetInserted(QList<Tileset *> const * value, int const position);
    void onTilesetRemoved(QList<Tileset *> const * value, int const position);
    void onTilesetMoved(QList<Tileset *> const * value, int const oldPosition, int const newPosition);

    void onEditorRootChanged(QPoint const value);
    void onReferenceOffsetChanged(QPoint const value);
    void onReferenceZoomChanged(int value);
    void onReferenceOffsetImageChanged(QImage * value);

    void onFilteredTilesChanged(QList<Tile *> const * value);
    void onFilteredPalettesChanged(QList<Palette *> const * value);
    void onFilteredTilesetsChanged(QList<Tileset *> const * value);

    void onShowLinkInfoChanged(bool value);

    // Scene
    void onSelectedSceneIDChanged(int sceneID);

    // Editor Toolbox
    void onEditorZoomChanged(int value);
    void onMoveViewport(int rx, int ry);
    void onMoveViewportHome();

    // References Toolbox
    void onReferenceModeChanged(ReferenceMode const value);

    // Tiles
    void onTilesFilterChanged(TilesFilter const & value);
    void onSelectedTilesChanged(QList<Tile*> const * tiles);
    void onMoveTileSelection(int rx, int ry);

    // Palettes
    void onPalettesModeChanged(PaletteMode value);
    void onSelectedPaletteChanged(Palette * value);

    // Tile Preview
    void onTileFilterChanged(TilePreviewFilter const & value);

    // Tilesets
    void onSelectedTilesetChanged(Tileset * value);

};

#endif // APPSTATE_H


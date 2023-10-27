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

enum EditorTool
{
    PENCIL,
    ERASER,
    LINKER
};

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

class TileMode
{
public:
    bool hFlip;
    bool vFlip;

public:
    TileMode() : hFlip(false), vFlip(false) {}

    bool operator!=(TileMode const & other) const
    {
        return !(*this == other);
    }

    bool operator==(TileMode const & other) const
    {
        return hFlip == other.hFlip && vFlip == other.vFlip;
    }
};


class AppState : public QObject
{
    friend class ServiceContext;

    friend class TilesFilterCommand;
    friend class SelectScenePosCommand;
    friend class SetSelectedTilesPosCommand;

private:

    Q_OBJECT

    // Context Folder
    Project            * _project;
    bool                 _projectHasChanges;
    QString              _projectLastDumpFolder;
    QList<Tile*>       * _projectTiles;
    QList<Palette*>    * _projectPalettes;
    QList<Tileset*>    * _projectTilesets;
    QList<Reference*>  * _projectReferences;
    QList<Screenshot*> * _projectScreenshots;
    QList<Scene*>      * _projectScenes;
    int                  _selectedScenePos;
    int                  _lastMoveToSceneResult;

    QHash<int, Tile*>       _index_Tile_ID;
    QHash<int, Tileset*>    _index_Tileset_ID;
    QHash<int, Screenshot*> _index_Screenshot_ID;
    QHash<int, Palette*>    _index_Palette_ID;
    QHash<int, Reference*>  _index_Reference_ID;
    QHash<int, Scene*>      _index_Scene_ID;


    // Editor Toolbox
    EditorTool _editorTool;
    bool       _editorShowLinkedTiles;
    bool       _editorShowUnlinkedTiles;
    bool       _editorShowGrid;
    QPoint     _editorRoot;
    int        _editorZoom;

    // References Toolbox
    ReferenceMode   _referenceMode;
    int             _referenceZoom;
    QImage        * _referenceOffsetImage;
    QPoint          _referenceOffset;

    // Tiles
    TilesFilter    _tilesFilter;
    Range _selectedTilesPos;

    // Palettes
    PaletteMode   _palettesMode;
    Palette     * _selectedPalette;

    // Tile Preview
    TileMode _tileMode;

    // Tilesets
    Tileset * _selectedTileset;

public:

    AppState();

    // Context Folder
    Project * project() const;
    bool projectHasChanges();
    QString const & projectLastDumpFolder() const;
    QList<Tile *> * projectTiles() const;
    QList<Palette *> * projectPalettes() const;
    QList<Tileset *> * projectTilesets() const;
    QList<Reference *> * projectReferences() const;
    QList<Screenshot *> * projectScreenshots() const;
    QList<Scene *> * projectScenes() const;
//    int selectedSceneID() const;

    void setProject(Project * value);
    void setProjectHasChanges(bool value);
    void setProjectLastDumpFolder(QString value);
    void setProjectTiles(QList<Tile *> * value);
    void setProjectPalettes(QList<Palette *> * value);
    void setProjectTilesets(QList<Tileset *> * value);
    void setProjectReferences(QList<Reference *> * value);
    void setProjectScreenshots(QList<Screenshot *> * value);
    void setProjectScenes(QList<Scene*> * value);

    Tile * getProjectTileById(int id);
    Palette * getProjectPaletteById(int id);
    Tileset * getProjectTilesetById(int id);
    Reference* getProjectReferenceById(int id);
    Screenshot * getProjectScreenshotById(int id);
    Scene * getProjectSceneById(int id);

    void appendProjectTile(Tile * value);
    void appendProjectPalette(Palette * value);
    void appendProjectTileset(Tileset * value);
    void appendProjectReference(Reference * value);
    void appendProjectScreenshot(Screenshot * value);
    void appendProjectScene(Scene * value);

    void insertProjectScene(int const position, Scene * value);
    void removeProjectScene(int const position);
    void moveUpProjectScene(int const position);
    void moveDownProjectScene(int const position);

    void insertProjectTileset(int const position, Tileset * value);
    void removeProjectTileset(int const position);
    void moveUpProjectTileset(int const position);
    void moveDownProjectTileset(int const position);

    void setLastMoveToSceneResult(int const value);
    int lastMoveToSceneResult();

    // Scene
    int selectedScenePos();
    int scenePos2ID(int scenePos);

    // Editor Toolbox
    EditorTool editorTool() const;
    bool editorShowLinkedTiles() const;
    bool editorShowUnlinkedTiles() const;
    bool editorShowGrid() const;

    void setEditorTool(EditorTool const value);
    void setEditorShowLinkedTiles(bool value);
    void setEditorShowUnlinkedTiles(bool value);
    void setEditorShowGrid(bool value);

    QPoint editorRoot();
    void moveEditorRoot(int rx, int ry);
    void moveEditorRootHome();

    void zoomInEditor();
    void zoomOutEditor();
    int editorZoom();

    void moveViewport(int rx, int ry);
    void moveViewportHome();

    void editorPaintCellUsingSibling();
    void editorPaintCellUsingSelection(int x, int y);
    void editorEraseCell(int x, int y);
    void editorColorPickCell(int x, int y);
    void editorLinkCell(int x, int y);
    void editorUndo();
    void editorRedo();

    // References Toolbox
    ReferenceMode referenceMode() const;
    QImage * referenceOffsetImage();
    int referenceZoom();

    void setReferenceMode(ReferenceMode value);
    void setReferenceHighlightPosition(bool value);
    void setReferenceOffsetImage(QImage * offsetImage);

    void zoomInReference();
    void zoomOutReference();

    QPoint referenceOffset();
    void moveReferenceOffset(int rx, int ry);
    void moveReferenceOffsetHome();

    // Tiles
    TilesFilter const & tilesFilter() const;
    Range const * selectedTilesPos() const;
    int selectedTilePos() const;
    QList<Tile*> selectedTiles() const;
    Tile * selectedTile() const;
    void moveSelectedTilesToScene(int sceneID);
    void selectTileAtEditorPosition(int rx, int ry);

    // Palettes
    PaletteMode palettesMode() const;
    Palette * selectedPalette();
    void setPalettesMode(PaletteMode value);
    void setSelectedPalette(Palette * value);

    // Tile Preview
    TileMode & tileMode();
    void setTileMode(TileMode const & value);

    // Tilesets
    Tileset * selectedTileset() const;
    void setSelectedTileset(Tileset * value);
    void moveSelectedTilesetToScene(int sceneID);

private:

    // Scenes
    void setSelectedScenePos(int value);

    //Tiles
    void setTilesFilter(TilesFilter const & value);
    void setSelectedTilesPos(Range value);

signals:

    // Context Folder
    void onProjectChanged(Project * value);
    void onProjectHasChangesChanged(bool value);
    void onProjectLastDumpFolderChanged(QString const & value);
    void onProjectTilesChanged(QList<Tile *> const * value);
    void onProjectPalettesChanged(QList<Palette *> const * value);
    void onProjectTilesetsChanged(QList<Tileset *> const * value);
    void onProjectReferencesChanged(QList<Reference *> const * value);
    void onProjectScreenshotsChanged(QList<Screenshot*> const * value);
    void onProjectScenesChanged(QList<Scene*> const * value);

    void onProjectScenesInserted(QList<Scene *> const * value, int const position);
    void onProjectScenesRemoved(QList<Scene *> const * value, int const position);
    void onProjectScenesMoved(QList<Scene *> const * value, int const oldPosition, int const newPosition);

    void onProjectTilesetsInserted(QList<Tileset *> const * value, int const position);
    void onProjectTilesetsRemoved(QList<Tileset *> const * value, int const position);
    void onProjectTilesetsMoved(QList<Tileset *> const * value, int const oldPosition, int const newPosition);

    void onEditorRootChanged(QPoint const value);
    void onReferenceOffsetChanged(QPoint const value);
    void onReferenceZoomChanged(int value);
    void onReferenceOffsetImageChanged(QImage * value);

    // Scene
    void onSelectedScenePosChanged(int value);

    // Editor Toolbox
    void onEditorToolChanged(EditorTool const value);
    void onEditorShowLinkedTilesChanged(bool const value);
    void onEditorShowUnlinkedTilesChanged(bool const value);
    void onEditorShowGridChanged(bool const value);
    void onEditorZoomChanged(int value);
    void onMoveViewport(int rx, int ry);
    void onMoveViewportHome();

    // References Toolbox
    void onReferenceModeChanged(ReferenceMode const value);

    // Tiles
    void onTilesFilterChanged(TilesFilter const & value);
    void onSelectedTilesPosChanged(Range value);
    void onSelectTileAtEditorPosition(int rx, int ry);

    // Palettes
    void onPalettesModeChanged(PaletteMode value);
    void onSelectedPaletteChanged(Palette * value);

    // Tile Preview
    void onTileModeChanged(TileMode const & value);

    // Tilesets
    void onSelectedTilesetChanged(Tileset * value);

};

#endif // APPSTATE_H


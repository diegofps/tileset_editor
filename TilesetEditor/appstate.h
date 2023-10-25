#ifndef APPSTATE_H
#define APPSTATE_H

#include "model/scene.h"
#include "model/project.h"
#include "model/reference.h"
#include "model/screenshot.h"
#include "model/tile.h"
#include "model/palette.h"
#include "model/tileset.h"

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
    REF_10,
    REF_100,
    REF_1000,
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
    int                  _selectedSceneID;
    int                  _lastMoveToSceneResult;

    QHash<int, Tile*>       _index_Tile_ID;
    QHash<int, Tileset*>    _index_Tileset_ID;
    QHash<int, Screenshot*> _index_Screenshot_ID;
    QHash<int, Palette*>    _index_Palette_ID;
    QHash<int, Reference*>  _index_Reference_ID;
    QHash<int, Scene*>      _index_Scene_ID;

    QPoint _editorRoot;
    QPoint _referenceOffset;

    // Editor Toolbox
    EditorTool _editorTool;
    bool       _editorShowLinkedTiles;
    bool       _editorShowUnlinkedTiles;
    bool       _editorShowGrid;

    // References Toolbox
    ReferenceMode _referenceMode;
    bool _referenceHighlightPosition;
    int _referenceZoom;
    QImage * _referenceOffsetImage;

    // Preview
//    QString _previewMode;

    // Tiles
    TilesFilter * _tilesFilter;
    QList<Tile*>  _selectedTiles;

    // Palettes
    PaletteMode   _palettesMode;
    Palette * _selectedPalette;

    // Tile Preview
//    QString _tilePreviewMode;
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
    int selectedSceneID() const;

    void setProject(Project * value);
    void setProjectHasChanges(bool value);
    void setProjectLastDumpFolder(QString value);
    void setProjectTiles(QList<Tile *> * value);
    void setProjectPalettes(QList<Palette *> * value);
    void setProjectTilesets(QList<Tileset *> * value);
    void setProjectReferences(QList<Reference *> * value);
    void setProjectScreenshots(QList<Screenshot *> * value);
    void setProjectScenes(QList<Scene*> * value);
    void setSelectedSceneID(int value);

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
    void moveUpProjectScene(const int position);
    void moveDownProjectScene(const int position);

    void insertProjectTileset(int const position, Tileset * value);
    void removeProjectTileset(int const position);
    void moveUpProjectTileset(const int position);
    void moveDownProjectTileset(const int position);

    void setLastMoveToSceneResult(int value);
    int lastMoveToSceneResult();

    void moveEditorRoot(int rx, int ry);
    void moveEditorRootHome();

    void moveReferenceOffset(int rx, int ry);
    void moveReferenceOffsetHome();

    void drawNearestReferenceTile();
    void drawTile();
    void eraseTile();
    void linkTile();
    void undo();
    void redo();

    // Editor Toolbox
    EditorTool editorTool() const;
    bool editorShowLinkedTiles() const;
    bool editorShowUnlinkedTiles() const;
    bool editorShowGrid() const;

    void setEditorTool(EditorTool const value);
    void setEditorShowLinkedTiles(bool value);
    void setEditorShowUnlinkedTiles(bool value);
    void setEditorShowGrid(bool value);

    // References Toolbox
    ReferenceMode referenceMode() const;
    bool referenceHighlightPosition() const;
    QImage * referenceOffsetImage();
    int referenceZoom();

    void setReferenceMode(ReferenceMode value);
    void setReferenceHighlightPosition(bool value);
    void setReferenceOffsetImage(QImage * offsetImage);

    void zoomInReference();
    void zoomOutReference();

    // Preview
//    const QString & previewMode() const;
//    void setPreviewMode(QString value);

    // Tiles
    TilesFilter * tilesFilter() const;
    QList<Tile*> const * selectedTiles() const;
    void setTilesFilter(TilesFilter * value);
    void setSelectedTiles(QList<Tile*> * value);
    void tilesMoveSelectedTilesToScene(int sceneID);

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
    void onSelectedSceneIDChanged(int value);

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

    // Editor Toolbox
    void onEditorToolChanged(EditorTool const value);
    void onEditorShowLinkedTilesChanged(bool const value);
    void onEditorShowUnlinkedTilesChanged(bool const value);
    void onEditorShowGridChanged(bool const value);

    // References Toolbox
    void onReferenceModeChanged(ReferenceMode const value);
    void onReferenceHighlightPositionChanged(bool const value);

    // Preview
//    void onPreviewModeChanged(QString const & value);

    // Tiles
    void onTilesFilterChanged(TilesFilter * value);
    void onSelectedTilesChanged(QList<Tile*> * value);

    // Palettes
    void onPalettesModeChanged(PaletteMode value);
    void onSelectedPaletteChanged(Palette * value);

    // Tile Preview
    void onTileModeChanged(TileMode const & value);

    // Tilesets
    void onSelectedTilesetChanged(Tileset * value);

};

#endif // APPSTATE_H


#ifndef APPSTATE_H
#define APPSTATE_H

#include "model/project.h"
#include "model/reference.h"
#include "model/screenshot.h"
#include "model/tile.h"
#include "model/palette.h"
#include "model/tileset.h"

#include <QObject>
#include <string>

enum EditorTool
{
    PENCIL,
    ERASER,
    LINKER
};

class AppState : public QObject
{
private:

    Q_OBJECT

    // Context Folder
    Project * _project;
//    QString _projectFolder; // TODO: Move to Project?
    QString _projectLastDumpFolder;
    QList<Tile*> * _projectTiles;
    QList<Palette*> * _projectPalettes;
    QList<Tileset*> * _projectTilesets;
    QList<Reference*> * _projectReferences;
    QList<Screenshot*> * _projectScreenshots;

    // Editor Toolbox
    EditorTool _editorTool;
    bool _editorShowLinkedTiles;
    bool _editorShowUnlinkedTiles;
    bool _editorShowGrid;

    // References Toolbox
    int _referenceScreenshot;
    bool _referenceHighlightPosition;

    // Preview
    QString _previewPage;

    // Tiles
    QString _tilesShow;

    // Palettes
    QString _palettesShow;
    Palette * _palettesSelectedItem;

    // Tile Preview
    QString _tilePreviewShow;

    //Tilesets
    Tileset * _tilesetsSelectedItem;

public:

    AppState();

    // Context Folder
    Project * project() const;
//    QString const & projectFolder() const;
    QString const & projectLastDumpFolder() const;
    QList<Tile *> * projectTiles() const;
    QList<Palette *> * projectPalettes() const;
    QList<Tileset *> * projectTilesets() const;
    QList<Reference *> * projectReferences() const;
    QList<Screenshot *> * projectScreenshots() const;

    void setProject(Project * value);
//    void setProjectFolder(QString value);
    void setProjectLastDumpFolder(QString value);
    void setProjectTiles(QList<Tile *> * value);
    void setProjectPalettes(QList<Palette *> * value);
    void setProjectTilesets(QList<Tileset *> * value);
    void setProjectReferences(QList<Reference *> * value);
    void setProjectScreenshots(QList<Screenshot *> * value);

    void insertProjectTileset(int const position, Tileset * value);
    void removeProjectTileset(int const position);
    void moveUpProjectTileset(const int position);
    void moveDownProjectTileset(const int position);

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
    int referenceScreenshot() const;
    bool referenceHighlightPosition() const;
    void setReferenceScreenshot(int value);
    void setReferenceHighlightPosition(bool value);

    // Preview
    const QString & previewPage() const;
    void setPreviewPage(QString value);

    // Tiles
    const QString & tilesShow() const;
    void setTilesShow(QString const & value);

    // Palettes
    const QString & palettesShow() const;
    Palette * palettesSelectedItem();
    void setPalettesShow(QString const & value);
    void setPalettesSelectedItem(Palette * value);

    // Tile Preview
    const QString & tilePreviewShow() const;
    void setTilePreviewShow(QString const & value);

    // Tilesets
    Tileset * tilesetsSelectedItem() const;
    void setTilesetsSelectedItem(Tileset * value);

signals:

    // Context Folder
    void onProjectChanged(Project * value);
//    void onProjectFolderChanged(QString const & value);
    void onProjectLastDumpFolderChanged(QString const & value);
    void onProjectTilesChanged(QList<Tile *> const * value);
    void onProjectPalettesChanged(QList<Palette *> const * value);
    void onProjectTilesetsChanged(QList<Tileset *> const * value);
    void onProjectReferencesChanged(QList<Reference *> const * value);
    void onProjectScreenshotsChanged(QList<Screenshot*> const * value);

    void onProjectTilesetsInserted(QList<Tileset *> const * value, int const position);
    void onProjectTilesetsRemoved(QList<Tileset *> const * value, int const position);
    void onProjectTilesetsMoved(QList<Tileset *> const * value, int const oldPosition, int const newPosition);

    // Editor Toolbox
    void onEditorToolChanged(EditorTool const value);
    void onEditorShowLinkedTilesChanged(bool const value);
    void onEditorShowUnlinkedTilesChanged(bool const value);
    void onEditorShowGridChanged(bool const value);

    // References Toolbox
    void onReferenceScreenshotChanged(int const value);
    void onReferenceHighlightPositionChanged(bool const value);

    // Preview
    void onPreviewPageChanged(QString const & value);

    // Tiles
    void onTilesShowChanged(QString const & value);

    // Palettes
    void onPalettesShowChanged(QString const & value);
    void onPaletteSelectedItemChanged(Palette * value);

    // Tile Preview
    void onTilePreviewShowChanged(QString const & value);

    // Tilesets
    void onTilesetsSelectedItemChanged(Tileset * value);

};

#endif // APPSTATE_H


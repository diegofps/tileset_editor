#ifndef APPSTATE_H
#define APPSTATE_H

#include "model/context.h"
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
    Context * _context;
    QString _contextFolder;
    QList<Tile*> * _contextTiles;
    QList<Palette*> * _contextPalettes;
    QList<Tileset*> * _contextTilesets;
    QList<Reference*> * _contextReferences;
    QList<Screenshot*> * _contextScreenshots;

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

    // Tile Preview
    QString _tilePreviewShow;

public:

    AppState();

    // Context Folder
    Context * context() const;
    QString const & contextFolder() const;
    QList<Tile *> * contextTiles() const;
    QList<Palette *> * contextPalettes() const;
    QList<Tileset *> * contextTilesets() const;
    QList<Reference *> * contextReferences() const;
    QList<Screenshot *> * contextScreenshots() const;

    void setContext(Context * value);
    void setContextFolder(QString value);
    void setContextTiles(QList<Tile *> * value);
    void setContextPalettes(QList<Palette *> * value);
    void setContextTilesets(QList<Tileset *> * value);
    void setContextReferences(QList<Reference *> * value);


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
    void setPalettesShow(QString const & value);

    // Tile Preview
    const QString & tilePreviewShow() const;
    void setTilePreviewShow(QString const & value);

signals:

    // Context Folder
    void onContextChanged(Context * value);
    void onContextFolderChanged(QString const & value);
    void onContextTilesChanged(QList<Tile *> const * value);
    void onContextPalettesChanged(QList<Palette *> const * value);
    void onContextTilesetsChanged(QList<Tileset *> const * value);
    void onContextReferencesChanged(QList<Reference *> const * value);

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

    // Tile Preview
    void onTilePreviewShowChanged(QString const & value);

};

#endif // APPSTATE_H

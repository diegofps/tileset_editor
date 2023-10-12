#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <string>

enum EditorTool {
    PENCIL,
    ERASER,
    LINKER
};

class AppState : public QObject
{
private:

    Q_OBJECT

    // Context Folder
    QString _contextFolder;

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
    const QString & contextFolder() const;
    void setContextFolder(QString value);

    // Editor Toolbox
    EditorTool editorTool() const;
    bool editorShowLinkedTiles() const;
    bool editorShowUnlinkedTiles() const;
    bool editorShowGrid() const;

    void setEditorTool(EditorTool value);
    void setEditorShowLinkedTiles(bool value);
    void setEditorShowUnlinkedTiles(bool value);
    void setEditorShowGrid(bool value);

    // References Toolbox
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
    void onContextFolderChanged(QString const & value);

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

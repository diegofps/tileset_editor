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

    QString _contextFolder;

    // Editor Toolbox

    EditorTool _editorTool;
    bool _editorShowLinkedTiles;
    bool _editorShowUnlinkedTiles;
    bool _editorShowGrid;

    // References Toolbox

    int _referenceScreenshot;
    bool _referenceHighlightPosition;

public:

    AppState();

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

signals:

    void onContextFolderChanged(QString & value);

    // Editor Toolbox

    void onEditorToolChanged(EditorTool value);
    void onEditorShowLinkedTilesChanged(bool value);
    void onEditorShowUnlinkedTilesChanged(bool value);
    void onEditorShowGridChanged(bool value);

    // References Toolbox

    void onReferenceScreenshotChanged(int value);
    void onReferenceHighlightPositionChanged(bool value);

};

#endif // APPSTATE_H

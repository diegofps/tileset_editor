#ifndef REFSCREENSHOT_H
#define REFSCREENSHOT_H

#include <QString>

class RefScreenshot
{
private:

    int _id;    // Number of first frame it appeared, as XGFX.ReferenceScreenshotID
    int _frame; // Number of first frame it appeared, as IPPU.TotalEmulatedFrames
    int _x;     // X coordinate in the screenshot
    int _y;     // Y coordinate in the screenshot
    int _startLine; // The StartLine received by the Draw function
    int _lineCount; // The LineCount received by the Draw function
    int _colorPaletteID; // Number of the color palette used to paint the tile
    QString _MATH;    // Name of the MATH type used
    QString _PIXEL;   // Name of the PIEL type used
    QString _OP;      // Name of the OP type used
    QString _BPSTART; // Name of the BPSTART type used
    QString _TILE;    // Name of the TILE type used

public:

    RefScreenshot();

};

#endif // REFSCREENSHOT_H

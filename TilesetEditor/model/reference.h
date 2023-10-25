#ifndef REFERENCE_H
#define REFERENCE_H

#include <QString>
#include <QJsonObject>

class Reference
{
public:

    int id;
    uint32_t tile;
    int screenshotId;   // Number of first frame it appeared, as XGFX.ReferenceScreenshotID
    int frame;          // Number of first frame it appeared, as IPPU.TotalEmulatedFrames
    int x;              // X coordinate in the screenshot
    int y;              // Y coordinate in the screenshot
    int startLine;      // The StartLine received by the Draw function
    int lineCount;      // The LineCount received by the Draw function
    int colorPaletteID; // Number of the color palette used to paint the tile
    bool hFlip;         // Indicates this tile was flipped horizontally
    bool vFlip;         // Indicates this tile was flipped vertically
    QString MATH;       // Name of the MATH type used
    QString PIXEL;      // Name of the PIEL type used
    QString OP;         // Name of the OP type used
    QString BPSTART;    // Name of the BPSTART type used
    QString TILE;       // Name of the TILE type used

public:

    Reference(QJsonObject & data);
    QJsonObject exportAsJson() const;
    void clear();
    bool isEmpty() const;

};

#endif // REFERENCE_H

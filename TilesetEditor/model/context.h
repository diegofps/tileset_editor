#ifndef CONTEXT_H
#define CONTEXT_H

#include <QJsonObject>



class Context
{
public:

    Context();

    Context(QJsonObject data);

    // TODO: Save context info like state IDs (for tiles, tilesets, palettes, and screenshots) and splitter positions.
};

#endif // CONTEXT_H

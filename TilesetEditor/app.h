#ifndef APP_H
#define APP_H

#include "appstate.h"
#include "appstyles.h"
#include "tilecache.h"

class App
{
private:

    static AppState * appState;
    static AppStyles * appStyles;
    static TileCache * originalTileCache;

public:

    App();
    static AppState * getState();
    static AppStyles * getStyles();
    static TileCache * getOriginalTileCache();

};

#endif // APP_H

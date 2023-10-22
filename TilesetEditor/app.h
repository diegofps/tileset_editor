#ifndef APP_H
#define APP_H

#include "appstate.h"
#include "appstyles.h"
#include "tilecache.h"
#include "tilecachehd.h"

class App
{
private:

    static AppState * appState;
    static AppStyles * appStyles;
    static TileCache * originalTileCache;
    static TileCacheHD * hdTileCache;

public:

    App();
    static AppState * getState();
    static AppStyles * getStyles();
    static TileCache * getOriginalTileCache();
    static TileCacheHD * getHDTileCache();

};

#endif // APP_H

#include "app.h"

AppState * App::appState = nullptr;
AppStyles * App::appStyles = nullptr;
TileCache * App::originalTileCache = nullptr;
TileCacheHD * App::hdTileCache = nullptr;

App::App()
{

}

AppState *App::getState()
{
    if (appState == nullptr)
        appState = new AppState();
    return appState;
}

AppStyles *App::getStyles()
{
    if (appStyles == nullptr)
        appStyles = new AppStyles();
    return appStyles;
}

TileCache *App::getOriginalTileCache()
{
    if (originalTileCache == nullptr)
        originalTileCache = new TileCache();
    return originalTileCache;
}

TileCacheHD *App::getHDTileCache()
{
    if (hdTileCache == nullptr)
        hdTileCache = new TileCacheHD();
    return hdTileCache;
}

#ifndef APP_H
#define APP_H

#include "appstate.h"
#include "appstyles.h"

class App
{
private:

    static AppState * appState;
    static AppStyles * appStyles;

public:

    App();

    static AppState & getState()
    {
        if (appState == nullptr)
            appState = new AppState();
        return *appState;
    }

    static AppStyles * getStyles()
    {
        if (appStyles == nullptr)
            appStyles = new AppStyles();
        return appStyles;
    }
};

#endif // APP_H

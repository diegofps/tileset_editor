#ifndef APP_H
#define APP_H

#include "appstate.h"

class App
{
private:

    static AppState * appState;

public:

    App();

    static AppState & getState()
    {
        if (appState == nullptr)
            appState = new AppState();
        return *appState;
    }

};

#endif // APP_H

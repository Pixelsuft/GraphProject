#define SDL_MAIN_USE_CALLBACKS 1
#include "app.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    res_init();
    App* app = new App;
    if (!app->is_inited()) {
        TTF_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    app->on_resize();
    app->setup_ui();
    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    App* app = reinterpret_cast<App*>(appstate);
    return app->on_event(*event) ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    App* app = reinterpret_cast<App*>(appstate);
    return app->on_update() ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (result != SDL_APP_FAILURE) {
        App* app = reinterpret_cast<App*>(appstate);
        delete app;
        TTF_Quit();
        SDL_Quit();
    }
}

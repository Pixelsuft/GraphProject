#pragma once
#include <SDL3/SDL_events.h>

class App {
    bool mouse_entered;

public:
    App();
    ~App();
    bool is_inited();
    bool on_event(SDL_Event& ev);
    bool on_update();
    void on_resize();
};

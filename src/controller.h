#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

struct _SDL_Joystick {
    SDL_JoystickID instance_id;
};

struct _SDL_GameController {
    SDL_Joystick *joystick;
    const char *name;
};

SDL_GameController *ControllerGetInstance(void) {
    static SDL_Joystick joystick = {
        .instance_id = 0,
    };

    static SDL_GameController gamecontroller = {
        .joystick = &joystick,
        .name = "X360 Controller",
    };

    return &gamecontroller;
}

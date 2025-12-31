#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

struct _SDL_Joystick {
    SDL_JoystickID instance_id;
} static _joystick = {
    .instance_id = 0,
};

struct _SDL_GameController {
    SDL_Joystick *joystick;
    const char *name;
} static _controller = {
    .joystick = &_joystick,
    .name = "X360 Controller",
};

SDL_GameController *ControllerGetInstance(void) {
    return &_controller;
}

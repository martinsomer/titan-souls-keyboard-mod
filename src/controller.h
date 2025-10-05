#include "SDL_joystick.h"
#include "SDL_gamecontroller.h"

struct _SDL_Joystick {
    SDL_JoystickID instance_id;
} static fake_joystick = {
    .instance_id = 0,
};

struct _SDL_GameController {
    SDL_Joystick *joystick;
    const char *name;
} static fake_controller = {
    .joystick = &fake_joystick,
    .name = "X360 Controller",
};

SDL_GameController *controller_get_instance(void) {
    return &fake_controller;
}

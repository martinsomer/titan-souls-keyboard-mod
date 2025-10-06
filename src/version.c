#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "version.h"
#include "MinHook.h"
#include "controller.h"
#include "keyboard.h"
#include "SDL.h"
#include "SDL_syswm.h"

HANDLE hThread = NULL;
HWND hGame = NULL;

typedef SDL_bool (SDLCALL *SDL_GetWindowWMInfo_t)(SDL_Window*, SDL_SysWMinfo*);
static SDL_GetWindowWMInfo_t pSDL_GetWindowWMInfo = NULL;

typedef SDL_Window *(SDLCALL *SDL_CreateWindow_t)(const char*, int, int, int, int, Uint32);
static SDL_CreateWindow_t pSDL_CreateWindow = NULL;
SDL_Window *SDLCALL _SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Window *window = pSDL_CreateWindow(title, x, y, w, h, flags);

    if (!hGame) {
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        pSDL_GetWindowWMInfo(window, &info);
        hGame = info.info.win.window;
    }

    return window;
}

typedef Uint32 (SDLCALL *SDL_GetTicks_t)(void);
static SDL_GetTicks_t pSDL_GetTicks = NULL;

typedef int (SDLCALL *SDL_PushEvent_t)(SDL_Event*);
static SDL_PushEvent_t pSDL_PushEvent = NULL;

SDL_bool SDLCALL _SDL_IsGameController(int joystick_index) {
    return joystick_index == 0 ? SDL_TRUE : SDL_FALSE;
}

SDL_GameController *SDLCALL _SDL_GameControllerOpen(int joystick_index) {
    return controller_get_instance();
}

const char *SDLCALL _SDL_GameControllerName(SDL_GameController *gamecontroller) {
    return gamecontroller->name;
}

SDL_Joystick *SDLCALL _SDL_GameControllerGetJoystick(SDL_GameController *gamecontroller) {
    return gamecontroller->joystick;
}

SDL_JoystickID SDLCALL _SDL_JoystickInstanceID(SDL_Joystick *joystick) {
    return joystick->instance_id;
}

SDL_Haptic *SDLCALL _SDL_HapticOpenFromJoystick(SDL_Joystick *joystick) {
    return NULL;
}

void controller_attach(void) {
    SDL_Event event = { 0 };

    event.cdevice.type = SDL_CONTROLLERDEVICEADDED;
    event.cdevice.timestamp = pSDL_GetTicks();
    event.cdevice.which = 0;

    pSDL_PushEvent(&event);
}

void controller_set_button(Uint8 button, BOOL state) {
    SDL_Event event = { 0 };

    event.cbutton.type = state ? SDL_CONTROLLERBUTTONDOWN : SDL_CONTROLLERBUTTONUP;
    event.cbutton.timestamp = pSDL_GetTicks();
    event.cbutton.which = 0;
    event.cbutton.button = button;
    event.cbutton.state = state ? SDL_PRESSED : SDL_RELEASED;

    pSDL_PushEvent(&event);
}

void controller_set_axis(Uint8 axis, Sint16 value) {
    SDL_Event event = { 0 };

    event.caxis.type = SDL_CONTROLLERAXISMOTION;
    event.caxis.timestamp = pSDL_GetTicks();
    event.caxis.which = 0;
    event.caxis.axis = axis;
    event.caxis.value = value;

    pSDL_PushEvent(&event);
}

DWORD WINAPI InputThread(LPVOID param) {
    controller_attach();

    while (TRUE) {
        if (hGame && GetForegroundWindow() == hGame) {
            KEYBOARD_STATE *kbstate = keyboard_get_state();

            RECT rect;
            GetClientRect(hGame, &rect);
            POINT tl = { rect.left, rect.top };
            POINT br = { rect.right, rect.bottom };
            ClientToScreen(hGame, &br);
            ClientToScreen(hGame, &tl);
            RECT clip = { tl.x, tl.y, br.x, br.y };
            ClipCursor(&clip);

            POINT center = {
                tl.x + ((rect.right - rect.left) / 2),
                tl.y + ((rect.bottom - rect.top) / 2),
            };

            if (kbstate->fire.changed || kbstate->camera.changed) {
                if (kbstate->fire.state || kbstate->camera.state) {
                    SetCursorPos(center.x, center.y);
                }

                if (kbstate->fire.changed) {
                    controller_set_button(SDL_CONTROLLER_BUTTON_X, kbstate->fire.state);
                }

                if (!kbstate->fire.state) {
                    controller_set_axis(SDL_CONTROLLER_AXIS_LEFTX, 0);
                    controller_set_axis(SDL_CONTROLLER_AXIS_LEFTY, 0);
                }

                if (!kbstate->camera.state) {
                    controller_set_axis(SDL_CONTROLLER_AXIS_RIGHTX, 0);
                    controller_set_axis(SDL_CONTROLLER_AXIS_RIGHTY, 0);
                }
            }

            if (kbstate->fire.state || kbstate->camera.state) {
                POINT mouse;
                GetCursorPos(&mouse);

                Sint16 dx = mouse.x - center.x;
                Sint16 dy = mouse.y - center.y;

                dx = dx < -100 ? -100 : (dx > 100 ? 100 : dx);
                dy = dy < -100 ? -100 : (dy > 100 ? 100 : dy);

                Sint16 x = dx * 32767 / 100;
                Sint16 y = dy * 32767 / 100;

                if (kbstate->fire.state) {
                    controller_set_axis(SDL_CONTROLLER_AXIS_LEFTX, x);
                    controller_set_axis(SDL_CONTROLLER_AXIS_LEFTY, y);
                }

                if (kbstate->camera.state) {
                    controller_set_axis(SDL_CONTROLLER_AXIS_RIGHTX, x);
                    controller_set_axis(SDL_CONTROLLER_AXIS_RIGHTY, y);
                }
            }

            if (kbstate->roll.changed) {
                controller_set_button(SDL_CONTROLLER_BUTTON_A, kbstate->roll.state);
            }

            if ((kbstate->left.changed || kbstate->right.changed || kbstate->fire.changed) && !kbstate->fire.state) {
                Sint16 x = kbstate->left.state * -32767 + kbstate->right.state * 32767;
                controller_set_axis(SDL_CONTROLLER_AXIS_LEFTX, x);
            }

            if ((kbstate->up.changed || kbstate->down.changed || kbstate->fire.changed) && !kbstate->fire.state) {
                Sint16 y = kbstate->up.state * -32767 + kbstate->down.state * 32767;
                controller_set_axis(SDL_CONTROLLER_AXIS_LEFTY, y);
            }
        }

        Sleep(16);
    }

    return 0;
}

void on_attach(void) {
    MH_Initialize();

    HMODULE hSDL = GetModuleHandleA("SDL2.dll");

    // Hook window methods for mouse capture
    pSDL_GetWindowWMInfo = (SDL_GetWindowWMInfo_t)GetProcAddress(hSDL, "SDL_GetWindowWMInfo");
    MH_CreateHook(GetProcAddress(hSDL, "SDL_CreateWindow"), _SDL_CreateWindow, (LPVOID *)&pSDL_CreateWindow);

    // Hook event loop methods to feed input
    pSDL_GetTicks = (SDL_GetTicks_t)GetProcAddress(hSDL, "SDL_GetTicks");
    pSDL_PushEvent = (SDL_PushEvent_t)GetProcAddress(hSDL, "SDL_PushEvent");

    // Hook controller methods to create fake device
    MH_CreateHook(GetProcAddress(hSDL, "SDL_IsGameController"), _SDL_IsGameController, NULL);
    MH_CreateHook(GetProcAddress(hSDL, "SDL_GameControllerOpen"), _SDL_GameControllerOpen, NULL);
    MH_CreateHook(GetProcAddress(hSDL, "SDL_GameControllerName"), _SDL_GameControllerName, NULL);
    MH_CreateHook(GetProcAddress(hSDL, "SDL_GameControllerGetJoystick"), _SDL_GameControllerGetJoystick, NULL);
    MH_CreateHook(GetProcAddress(hSDL, "SDL_JoystickInstanceID"), _SDL_JoystickInstanceID, NULL);
    MH_CreateHook(GetProcAddress(hSDL, "SDL_HapticOpenFromJoystick"), _SDL_HapticOpenFromJoystick, NULL);

    MH_EnableHook(MH_ALL_HOOKS);

    hThread = CreateThread(NULL, 0, InputThread, NULL, 0, NULL);
}

void on_detach(void) {
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        hThread = NULL;
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

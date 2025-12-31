#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct {
    BOOL state;
    BOOL prev;
    BOOL changed;
} KEY_STATE;

typedef struct {
    KEY_STATE fire;
    KEY_STATE camera;
    KEY_STATE roll;
    KEY_STATE left;
    KEY_STATE right;
    KEY_STATE up;
    KEY_STATE down;
} KEYBOARD_STATE;

static KEYBOARD_STATE _kbstate;

static void KeyboardUpdateKey(KEY_STATE *kstate, int code) {
    kstate->prev = kstate->state;
    kstate->state = (GetAsyncKeyState(code) & 0x8000) != 0;
    kstate->changed = kstate->state != kstate->prev;
}

KEYBOARD_STATE *KeyboardGetState(void) {
    KeyboardUpdateKey(&_kbstate.fire, 0x01);
    KeyboardUpdateKey(&_kbstate.camera, 0x02);
    KeyboardUpdateKey(&_kbstate.roll, 0x20);
    KeyboardUpdateKey(&_kbstate.left, 0x41);
    KeyboardUpdateKey(&_kbstate.right, 0x44);
    KeyboardUpdateKey(&_kbstate.up, 0x57);
    KeyboardUpdateKey(&_kbstate.down, 0x53);

    return &_kbstate;
}

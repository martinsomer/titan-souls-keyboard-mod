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

static void KeyboardUpdateKey(KEY_STATE *kstate, int code) {
    kstate->prev = kstate->state;
    kstate->state = (GetAsyncKeyState(code) & 0x8000) != 0;
    kstate->changed = kstate->state != kstate->prev;
}

KEYBOARD_STATE *KeyboardGetState(void) {
    static KEYBOARD_STATE kbstate;

    KeyboardUpdateKey(&kbstate.fire, 0x01);
    KeyboardUpdateKey(&kbstate.camera, 0x02);
    KeyboardUpdateKey(&kbstate.roll, 0x20);
    KeyboardUpdateKey(&kbstate.left, 0x41);
    KeyboardUpdateKey(&kbstate.right, 0x44);
    KeyboardUpdateKey(&kbstate.up, 0x57);
    KeyboardUpdateKey(&kbstate.down, 0x53);

    return &kbstate;
}

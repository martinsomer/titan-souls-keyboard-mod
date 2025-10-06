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

static inline void update_key_state(KEY_STATE *keystate, int key) {
    keystate->prev = keystate->state;
    keystate->state = (GetAsyncKeyState(key) & 0x8000) != 0;
    keystate->changed = keystate->state != keystate->prev;
}

KEYBOARD_STATE *keyboard_get_state(void) {
    update_key_state(&_kbstate.fire, 0x01);
    update_key_state(&_kbstate.camera, 0x02);
    update_key_state(&_kbstate.roll, 0x20);
    update_key_state(&_kbstate.left, 0x41);
    update_key_state(&_kbstate.right, 0x44);
    update_key_state(&_kbstate.up, 0x57);
    update_key_state(&_kbstate.down, 0x53);

    return &_kbstate;
}

#include <string.h>
#include <gba.h>

#include "place.h"

inline int min (int a, int b) { return a < b ? a : b; }
inline int max (int a, int b) { return a > b ? a : b; }

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

int cx = 0;
int cy = 0;

int main(void) {
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    for (int y = 0; y < 160; y++) {
        dmaCopy(&place[y*1000], (u16*)(VRAM + y*240*2), 240*2);
    }

    while (true) {
        u8 keys = ~REG_KEYINPUT;

        if (keys) {
            int d = 1;
            if (keys & ((1 << 0) | (1 << 1))) d = 3;
            if (keys & (1 << 6)) cy -= d;
            if (keys & (1 << 7)) cy += d;
            if (keys & (1 << 5)) cx -= d;
            if (keys & (1 << 4)) cx += d;

            cx = constrain(cx, 0, 1000 - 240);
            cy = constrain(cy, 0, 1000 - 160);

            for (int y = 0; y < 160; y++) {
                dmaCopy(&place[(y + cy)*1000 + cx], (u16*)(VRAM + y*240*2), 240*2);
            }
        }

        VBlankIntrWait();
    }
}

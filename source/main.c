#include <string.h>
#include <gba.h>

#include "place.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

int cx = 0;
int cy = 0;

int main(void) {
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    for (int y = 0; y < 160; y++) {
        dmaCopy(&place[y*place_width], (u16*)(VRAM + y*240*2), 240*2);
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

            cx = constrain(cx, 0, place_width - 240);
            cy = constrain(cy, 0, place_height - 160);

            for (int y = 0; y < 160; y++) {
                dmaCopy(&place[(y + cy)*place_width + cx], (u16*)(VRAM + y*240*2), 240*2);
            }
        }

        VBlankIntrWait();
    }
}

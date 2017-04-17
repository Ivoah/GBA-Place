#include <string.h>
#include <gba.h>

#include "place.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

int cx = 0;
int cy = 0;

BGAffineSource zoom = {
    //Center Of Rotation In Original Image (Last 8-Bits Fractional)
    .x = 0x00007800, // X
    .y = 0x00005000, // Y
    // Center Of Rotation On Screen
    .tX = 0x0078, // X
    .tY = 0x0050, // Y
    // Scaling Ratios (Last 8-Bits Fractional)
    .sX = 0x0100, // X
    .sY = 0x0100, // Y
    // Angle Of Rotation ($0000..$FFFF Anti-Clockwise)
    .theta = 0x0000,
};

int main(void) {
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    for (int y = 0; y < 160; y++) {
        dmaCopy(&place[y*place_width], (u16*)(VRAM + y*240*2), 240*2);
    }

    while (true) {
        u16 keys = ~REG_KEYINPUT;

        if (keys) {
            int d = 1;
            if (keys & ((1 << 0) | (1 << 1))) d = 3;
            if (keys & (1 << 6)) cy -= d;
            if (keys & (1 << 7)) cy += d;
            if (keys & (1 << 5)) cx -= d;
            if (keys & (1 << 4)) cx += d;
            if (keys & (1 << 8)) {zoom.sX -= 4; zoom.sY -= 4;}
            if (keys & (1 << 9)) {zoom.sX += 4; zoom.sY += 4;}
            
            cx = constrain(cx, 0, place_width - 240);
            cy = constrain(cy, 0, place_height - 160);

            for (int y = 0; y < 160; y++) {
                dmaCopy(&place[(y + cy)*place_width + cx], (u16*)(VRAM + y*240*2), 240*2);
                //memcpy((u16*)(VRAM + y*240*2), &place[(y + cy)*place_width + cx], 240*2);
            }
            VBlankIntrWait();

            BgAffineSet(&zoom, (BGAffineDest*)&REG_BG2PA, 1);
        }

        //VBlankIntrWait();
    }
}

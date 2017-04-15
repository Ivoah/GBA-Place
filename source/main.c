#include <string.h>
#include <gba.h>

#include "place.h"

int cx = 0;
int cy = 0;

bool update = true;

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	REG_DISPCNT = MODE_3 | BG2_ENABLE;

	while (true) {
        u8 keys = ~REG_KEYINPUT;
		int d = 1;
		if (keys != 0xff) update = true;
		if (keys & ((1 << 0) | (1 << 1))) d = 3;
        if (keys & (1 << 6) && cy > 0) cy -= d;
        if (keys & (1 << 7) && cy < 1000 - 160) cy += d;
        if (keys & (1 << 5) && cx > 0) cx -= d;
        if (keys & (1 << 4) && cx < 1000 - 240) cx += d;

		if (update) {
			for (int y = 0; y < 160; y++) {
				dmaCopy(&place[(y + cy)*1000 + cx], (u16*)(VRAM + y*240*2), 240*2);
			}
			update = false;
		}
    }
}

#include "epicardium.h"

#include <math.h>
#include <stdio.h>

int levels[11]         = { 0 };
int levels_display[11] = { 0 };

void fade()
{
	for (int i = 0; i < 11; i++) {
		int level = levels[i];
		if (levels_display[i] > 0) {
			epic_leds_set(i, level, 0, 0);
			if (level == 0) {
				levels_display[i] = 0;
			}
		}
		if (levels[i] > 0) {
			levels[i]--;
		}
	}
}

/*
 * main() is called when l0dable is loaded and executed.
 */
int main(void)
{
	printf("Hello from blinky!\n");

	epic_leds_dim_top(6);

	// l0dables are running on a separate, exclusive-to-l0dables core.
	// Busy-waiting will not block the main operating system on core0 from
	// running - but it will drain batteries.
	for (;;) {
		for (int i = 0; i < 11; i++) {
			levels[i]         = 128;
			levels_display[i] = 1;
			for (int j = 0; j < 32; j++) {
				fade();
			}
		}
		for (int i = 9; i > 0; i--) {
			levels[i]         = 128;
			levels_display[i] = 1;
			for (int j = 0; j < 32; j++) {
				fade();
			}
		}
	}
}

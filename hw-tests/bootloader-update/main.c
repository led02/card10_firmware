/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "card10.h"
#include "bootloader-9251ea6.h"
#include "display.h"
#include "gfx.h"
#include "pmic.h"

#include "flc.h"
#include "icc.h"
#include "tmr_utils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/***** Definitions *****/

/***** Globals *****/
static void pmic_button(bool falling)
{
	if (falling) {
		printf("Resetting ...\n");
		/*
		 * Give the UART fifo time to clear.
		 * TODO: Do this properly
		 */
		for (int i = 0; i < 0x1000000; i++) {
			__asm volatile("nop");
		}
		MXC_GCR->rstr0 = MXC_F_GCR_RSTR0_SYSTEM;
	}
}

int main(void)
{
	card10_init();
	pmic_set_button_callback(pmic_button);

	printf("Erasing bootloader.\n");
	txt_puts(&display_textb, "Erasing bootloader.\n");

	ICC_Disable();

	int ret = FLC_MultiPageErase(0x10000000, 0x10000000 + 1024 * 64 - 1);
	if (ret != E_NO_ERROR) {
		printf("FLC_MultiPageErase failed with %d\n", ret);
		txt_puts(&display_textb, "Fail.\n");
		while (1)
			;
	}

	printf("Writing bootloader.\n");
	txt_puts(&display_textb, "Writing bootloader.\n");

	ret = FLC_Write(
		0x10000000,
		sizeof(bootloader_data),
		(uint32_t *)bootloader_data
	);
	if (ret != E_NO_ERROR) {
		printf("FLC_Write failed with %d\n", ret);
		txt_puts(&display_textb, "Fail.\n");
		while (1)
			;
	}
	ICC_Enable();

	printf("Done.\n");
	txt_puts(&display_textb, "Done.\n");
	txt_puts(&display_textb, "Please restart.\n");

	while (1) {
		card10_poll();
	}
}

/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "card10.h"
#include "bootloader-9251ea6.h"
#include "display.h"
#include "GUI_Paint.h"
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
    Paint_DrawString_EN(0, 16*0, "Erasing bootloader", &Font16, 0x0000, 0xffff);
    LCD_Update();

    ICC_Disable();

    int ret = FLC_MultiPageErase(0x10000000, 0x10000000 + 1024*64-1);
    if(ret != E_NO_ERROR) {
        printf("FLC_MultiPageErase failed with %d\n", ret);
        Paint_DrawString_EN(0, 16*1, "Fail.", &Font16, 0x0000, 0xffff);
        LCD_Update();
        while(1);
    }


    printf("Writing bootloader.\n");
    Paint_DrawString_EN(0, 16*0, "Writing bootloader ", &Font16, 0x0000, 0xffff);
    LCD_Update();

    ret = FLC_Write(0x10000000, sizeof(bootloader_data), (uint32_t *)bootloader_data);
    if(ret != E_NO_ERROR) {
        printf("FLC_Write failed with %d\n", ret);
        Paint_DrawString_EN(0, 16*1, "Fail.", &Font16, 0x0000, 0xffff);
        LCD_Update();
        while(1);
    }
    ICC_Enable();

    printf("Done.\n");
    Paint_DrawString_EN(0, 16*1, "Done.", &Font16, 0x0000, 0xffff);
    Paint_DrawString_EN(0, 16*2, "Please restart", &Font16, 0x0000, 0xffff);
    LCD_Update();

    while (1) {
        card10_poll();
    }
}

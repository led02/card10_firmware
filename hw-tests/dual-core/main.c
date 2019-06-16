/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "pmic.h"
#include "leds.h"
#include "card10.h"

#include "GUI_Paint.h"

#include "tmr_utils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Heart.h>

int main(void)
{
    card10_init();
    card10_diag();

    Paint_DrawImage(Heart, 0, 0, 160, 80);
    LCD_Update();

    for(int i=0; i<11; i++) {
        leds_set_dim(i, 1);
    }

    int h = 0;

    // Release core1
    core1_start();

    while (1) {
        #define NUM     15
        for(int i=0; i<NUM; i++) {
            if(i < 12) {
                leds_set_hsv(i, (h + 360/NUM * i) % 360, 1., 1./8);
            } else {
                leds_set_hsv(i, (h + 360/NUM * i) % 360, 1., 1.);
            }
        }

        leds_update();
        TMR_Delay(MXC_TMR0, MSEC(10), 0);
        h++;

        // Send a txev using `sev` every once in a while to wake up core1
        // and let it do something
        if (h % 100 == 0) {
            printf("core0: Triggering core1 using SEV ...\n");
            __asm volatile("sev");
        }
    }
}

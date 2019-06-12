/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "tmr_utils.h"
#include "gpio.h"
#include "GUI_DEV/GUI_Paint.h"
#include "Fonts/fonts.h"
#include "image/image.h"
#include "tmr.h"

#include "card10.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>


// *****************************************************************************
int main(void)
{
    int count = 0;

    card10_init();
    card10_diag();

    Paint_DrawString_EN(0, 0, "123", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 23, "ABC", &Font24, BLUE, CYAN);
    Paint_DrawString_CN(20,42, "Î¢Ñ©µç×Ó",  &Font24CN, WHITE, RED);
    Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
    Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawCircle(85,25, 22, GREEN, DRAW_FILL_EMPTY, DOT_PIXEL_2X2);

    Paint_DrawImage(gImage_40X40,120, 0,40, 40);
    Paint_DrawImage(gImage_160X80,0, 0, 160, 80);
    LCD_Update();
    DEV_Delay_ms(3000);

    while (1) {
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
        printf("count = %d\n", count++);
    }
}

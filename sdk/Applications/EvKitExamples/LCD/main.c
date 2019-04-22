/**
 * @file    main.c
 * @brief   LCD Demo
 * @details Demonstrates the use of the EvKit LCD (LS013B7DH03)
 */

/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2019-01-10 00:12:16 +0000 (Thu, 10 Jan 2019) $
 * $Revision: 40315 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pb.h"
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "uart.h"
#include "spi.h"

#include "lvgl.h"
#include "lv_hal_tick.h"

/***** Definitions *****/
#define SPI_SPEED       1000000  // Bit Rate (works, should be maximum for display)

#define SPI SPI0
#define SPI_IRQ SPI0_IRQn

/* Time (ms) between moving text location
 * Lower bound set by display update over SPI and 1ms SysTick rate
 */
#define TEXT_BOUNCE_DELAY 10

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define BITREVERSE_UINT8(x) ( \
  ((x & 0x01) << 7) | ((x & 0x02) << 5) | ((x & 0x04) << 3) | ((x & 0x08) << 1) | \
  ((x & 0x80) >> 7) | ((x & 0x40) >> 5) | ((x & 0x20) >> 3) | ((x & 0x10) >> 1))

/***** Globals *****/
uint8_t framebuffer[128*16];
gpio_cfg_t gpio_ssel0;
gpio_cfg_t gpio_displayon;

/***** Functions *****/

void SysTick_Handler(void)
{
  /* 1ms tick */
  lv_tick_inc(1);
}

void SPI0_IRQHandler(void)
{
  SPI_Handler(SPI);
}

unsigned int roll_led(void)
{
  static unsigned int state = 0;
  unsigned int max_state = (1<<num_leds)-1; /* 2^n - 1 */
  unsigned int i;
    
  if (num_leds) {
    for (i = 0; i < num_leds; i++) {
      if (state & (1<<i)) {
	LED_On(i);
      } else {
	LED_Off(i);
      }
    }
    if (++state > max_state) {
      state = 0;
    }
  }

  return state;
}

void update(uint8_t *arr)
{
  spi_req_t req;
  uint8_t rx_data[1+1+16+2];
  uint8_t tx_data[1+1+16+2];
  int i;
  int offset;

  // SSEL0 high
  GPIO_OutSet(&gpio_ssel0);

  for (i = 1; i <= 128; i++) {
      offset = 0;

      // Fast update only requires command byte on first row transfer.
      if (i == 1) {
	tx_data[offset++] = 0x80; // Command, update data
      }

      tx_data[offset++] = BITREVERSE_UINT8(i); // Address (high bit is in LSBit location)
      memcpy(tx_data+offset,arr,16);
      arr += 16;
      offset += 16;
      // 16 bits of dummy data.
      tx_data[offset++] = 0x00;

      // Fast update only requires command byte on first row transfer.
      if (i == 128) {
	tx_data[offset++] = 0x00; // Extra clocks
      }
      
      req.tx_data = tx_data;
      req.rx_data = rx_data;
      req.len = offset;
      req.bits = 8;
      req.width = SPI17Y_WIDTH_1;
      req.ssel = 0;
      req.deass = 1;
      req.tx_num = 0;
      req.rx_num = 0;
      req.callback = NULL;

      // Transfer data
      SPI_MasterTrans(SPI, &req);
    }

  // SSEL0 low
  GPIO_OutClr(&gpio_ssel0);
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
	if (color_p->full) {
	  framebuffer[(y*16)+(x/8)] |= (0x80>>(x&0x7));
	} else {
	  framebuffer[(y*16)+(x/8)] &= ~(0x80>>(x&0x7));
	}
	color_p++;
        }
    }
    update(framebuffer);

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}

int main(void)
{
  sys_cfg_spi_t spi17y_master_cfg;
  lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
  uint32_t last_tick, last_roll_tick;
  int x, y, incx, incy;
  lv_obj_t *label1, *label2, *led0, *led1;
  unsigned int state;

  /* setup the systick */
  SysTick->LOAD = (SystemCoreClock / 1000);
  SysTick->VAL = 0;
  SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
  
  // Turn on use of VDDOIH on P1.8,9,10,11
  MXC_GPIO1->vssel |= PIN_8 | PIN_9 | PIN_10 | PIN_11;
  
  // Initialize slave select 
  gpio_ssel0.port = PORT_1;
  gpio_ssel0.mask = PIN_8;
  gpio_ssel0.pad = GPIO_PAD_NONE;
  gpio_ssel0.func = GPIO_FUNC_OUT;
  GPIO_Config(&gpio_ssel0);
  GPIO_OutClr(&gpio_ssel0);


  spi17y_master_cfg.map = MAP_A;
  //    spi17y_master_cfg.ss0 = Enable;
  spi17y_master_cfg.ss0 = Disable;
  spi17y_master_cfg.ss1 = Disable;
  spi17y_master_cfg.ss2 = Disable;
   
  printf("\n\n***** " TOSTRING(TARGET) " LCD Example *****\n");

  // Configure the peripheral
  if (SPI_Init(SPI, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
    printf("Error configuring SPI\n");
    while (1) {}
  }

  // Initialize display on signal, take it back from SPI.
  gpio_displayon.port = PORT_1;
  gpio_displayon.mask = PIN_10;
  gpio_displayon.pad = GPIO_PAD_NONE;
  gpio_displayon.func = GPIO_FUNC_OUT;
  GPIO_Config(&gpio_displayon);
  GPIO_OutSet(&gpio_displayon);
  GPIO_OutClr(&gpio_displayon);
  GPIO_OutSet(&gpio_displayon);

  /* Clear the screen, also sends the update twice in case the LCD is not synchronized after reset */
  memset(framebuffer, 0xff, 128*16);
  update(framebuffer);
  update(framebuffer);

  /* LittlevGL setup */
  lv_init();
  lv_disp_drv_init(&disp_drv);
  disp_drv.disp_flush = ex_disp_flush;
  lv_disp_drv_register(&disp_drv);

  /* Generate text labels to bounce around the screen */
  label1 =  lv_label_create(lv_scr_act(), NULL);
  label2 =  lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(label1, "Maxim");
  lv_label_set_text(label2, "Integrated");

  /* Align the Label to the center
   * NULL means align on parent (which is the screen now)
   * 0, 0 at the end means an x, y offset after alignment*/
  lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_align(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

  /* Create simulated LEDs to mirror the physical LEDs */
  led0  = lv_led_create(lv_scr_act(), NULL);
  lv_obj_set_style(led0, &lv_style_pretty_color);
  lv_obj_set_size(led0, 20, 20);
  lv_obj_align(led0, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -5);
  lv_led_off(led0);
  led1  = lv_led_create(lv_scr_act(), NULL);
  lv_obj_set_style(led1, &lv_style_pretty_color);
  lv_obj_set_size(led1, 20, 20);
  lv_obj_align(led1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 30, -5);
  lv_led_off(led1);

  /* Bounce routine */
  last_tick = last_roll_tick = lv_tick_get();
  x = y = 0;
  incx = 1;
  incy = 2;
  while (1) {
    /* Create the bouncing text effect */
    if ((last_tick + TEXT_BOUNCE_DELAY) < lv_tick_get()) {
      last_tick = lv_tick_get();
      if (x >= 50) {
	incx = -1;
      } else if (x <= 25) {
	incx = 1;
      }
      x += incx;
      if (y >= 64) {
	incy = -2;
      } else if (y <= 0) {
	incy = 2;
      }
      y += incy;
      
      lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
      lv_obj_align(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }

    if ((last_roll_tick + 500) < lv_tick_get()) {
      last_roll_tick = lv_tick_get();
      state = roll_led();
      if (state & 1) {
	lv_led_on(led0);
      } else {
	lv_led_off(led0);
      }
      if (state & 2) {
	lv_led_on(led1);
      } else {
	lv_led_off(led1);
      }
    }

    if (last_tick != lv_tick_get()) {
      /* The timing is not critical but should be between 1..10 ms */
      lv_task_handler();
    }
    
    /* Sleep in low-power while nothing to do */
    __WFI();
  }
  
  return 0;
}

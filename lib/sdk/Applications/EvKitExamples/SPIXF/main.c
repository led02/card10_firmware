/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-12-03 22:54:18 +0000 (Mon, 03 Dec 2018) $
 * $Revision: 39578 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   SPIX example using the MX25.
 * @details Uses the MX25 on the EvKit to show the SPIX. Erases, writes, and then
 *          verifies the data. MX25_BAUD, MX25_ADDR, and MX25_SPIXFC_WIDTH
 *          can be changed to alter the communication between the devices. Refer
 *          to the schematic for the pinout and ensure that there are no switches
 *          blocking the communication to the MX25.
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "spixf_regs.h"
#include "mxc_sys.h"
#include "max32665.h"
#include "mx25.h"
#include "spixf.h"
#include "board.h"
#include "led.h"

/***** Definitions *****/

#define MX25_ADDR               0
#define MX25_BAUD			    4000000
#define MX25_SPIXFC_WIDTH		SPIXFC_WIDTH_4
#define MX25_EXP_ID 			0xC22537

int fail = 0;

/***** Functions *****/
void xip_function(void);
    
// These are set in the linkerfile and give the starting and ending address of xip_section
#if defined ( __GNUC__)
extern uint8_t __load_start_xip, __load_length_xip;
#elif defined ( __CC_ARM )
// Note: This demo has not been tested under IAR and should be considered non-functional
extern int Image$$RW_IRAM2$$Length;
extern char Image$$RW_IRAM2$$Base[];
uint8_t * __xip_addr;
#endif

/******************************************************************************/
void spixf_cfg_setup (spixf_req_t *req) {
  
    // cfg
    req->mode  = SPI_MODE_0;
    req->sspol = SSPOL_ACTIVE_LO;
    req->baud  = MX25_BAUD;  
    req->ssact = SYS_CLOCKS_2;
    req->ssiact = SYS_CLOCKS_3;
  
    // fetch_ctrl
    req->cmdval = MX25_CMD_QREAD;  // Flash Chip dependent... 
    req->cmdwth = SINGLE_SDIO_CMD;
    req->adrwth = QUAD_SDIO_ADDR;
    req->datwth = QUAD_SDIO_DATA;
    req->addr4  = THREE_BYTE_ADDR_MODE;
    
    // mode_ctrl
    req->mdclk = MX25_QREAD_DUMMY;  // Number of SPI clocks needed during mode/dummy phase of fetch
    req->nocmd = SND_RDCMD_EVRYTIME;
    
    // mode_data
    req->mddata = 0x0000;
    req->mdoe   = 0x0000;
    
    //fb_ctrl
    req->fbmd =  ENABLE_SCLK_FB;
    req->finv =  DISABLE_FBCLK_INV;
    
    req->busidle = 0x2000;  // 410us
}

/******************************************************************************/
int main(void)
{
    uint32_t id;
    spixf_req_t req;
    void(*func)(void);
    uint8_t rx_buf[(uint32_t)(&__load_length_xip)];
    int rx_len = sizeof(rx_buf);

    printf("\n\n***** SPIX Example *****\n");
    printf("This example communicates with an MX25\n");
    printf("flash on the EvKit, loads code onto it\n");
    printf("and then executes that code using the SPIX\n");
    printf("execute-in-place peripheral\n\n");

    printf("SPI Clock \t: %d Hz\n\n", MX25_BAUD);
    
    // Initialize the SPIXFC registers and set the appropriate output pins 
    if (MX25_Init() != E_NO_ERROR) {
        printf("BoardInit Fail\n");
        printf("Example Failed\n");
        while(1);
    }
    printf("MX25 Initialized.\n");

    // Get the ID of the MX25
    if ((id = MX25_ID()) == MX25_EXP_ID) {
	   printf("MX25 ID verified.\n");
    } else {
	   printf("Error verifying MX25 ID: 0x%06x\n", id);
       printf("Example Failed\n");
	   while (1);
    }
    
    int err;
    // Erase Test Sector
    printf("Erasing first 64k sector\n");
    if ((err = MX25_Erase(0x00000, MX25_Erase_32K)) != E_NO_ERROR) {
        printf("Error erasing flash: %d\n", err);
        printf("Example Failed\n");
        while(1);
    } else {
        printf("Erased\n");
    }
    
    // Enable Quad mode if we are using quad
    if (MX25_SPIXFC_WIDTH == SPIXFC_WIDTH_4) {
    	if((err =  MX25_Quad(1)) != E_NO_ERROR) {
	       printf("Error enabling quad mode: %d\n", err);
            fail++;
    	}
    } else {
        if(MX25_Quad(0) != E_NO_ERROR) {
            printf("Error disabling quad mode\n");
            fail++;
        }
    }

    // Program the MX25
    printf("Programming function (%d bytes @ 0x%08x) into external MX25 flash\n", (uint32_t)(&__load_length_xip), &__load_start_xip);
    if((err = MX25_Program_Page(MX25_ADDR, &__load_start_xip, (uint32_t)(&__load_length_xip), MX25_SPIXFC_WIDTH)) != E_NO_ERROR) {
        printf("Error Programming: %d\n", err);
        fail++;
    } else {
        printf("Programmed\n");
    }

    printf("Verifying external flash\n");
    if((err = MX25_Read(MX25_ADDR, rx_buf, rx_len, MX25_SPIXFC_WIDTH)) != E_NO_ERROR) {
        printf("Error verifying data %d\n", err);
        fail++;
    } else {
        if(memcmp(rx_buf, &__load_start_xip, rx_len) != E_NO_ERROR) {
            printf("Error invalid data\n");
            fail++;
        } else {
            printf("Verified\n");
        }
    }

    // Setup SPIX
    spixf_cfg_setup(&req);
    SPIXF_setup(MXC_SPIXF, &req);

    if(fail == 0) {
        printf("Jumping to external flash (@ 0x%08x), watch for blinking LED.\n", (MXC_XIP_MEM_BASE | 0x1));
        func = (void(*)(void))(MXC_XIP_MEM_BASE | 0x1);
        func();
        printf("Returned from external flash\n");
    }

    printf("\n");
    if(fail == 0) {
        printf("Example Succeeded\n");
    } else {
        printf("Example Failed\n");
    }
    return 0;
}
/*
 * @file
 * @brief   Demonstrates the various low power modes.
 *
 * @details Iterates through the various low power modes, using either the RTC
 *          alarm or a GPIO to wake from each.  #defines determine which wakeup
 *          source to use.  Once the code is running, you can measure the
 *          current used on the VCORE rail.
 *
 *          The power states shown are:
 *            1. Active mode power with all clocks on
 *            2. Active mode power with peripheral clocks disabled
 *            3. Active mode power with unused RAMs in light sleep mode
 *            4. Active mode power with unused RAMS shut down
 *            5. SLEEP mode
 *            6. BACKGROUND mode
 *            7. DEEPSLEEP mode
 *            8. BACKUP mode
 */

/* *****************************************************************************
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
 * $Date: 2019-04-26 14:44:33 -0500 (Fri, 26 Apr 2019) $
 * $Revision: 42977 $
 *
 **************************************************************************** */

#include <stdio.h>
#include <max32665.h>
#include <pwrseq_regs.h>
#include <tmr_utils.h>
#include "core_cm4.h"
#include "max32665.h"
#include "icc_regs.h"
#include "mxc_sys.h"
#include <gcr_regs.h>
#include "simo.h"
#include "rtc.h"

#include "nvic_table.h"
#include "pwrseq_regs.h"
#include "usbhs_regs.h"
#include "mcr_regs.h"
#include "gcr_regs.h"
#include "uart.h"
#include "lp.h"
#include "icc.h"
#include "board.h"
#include "pb.h"
#include "led.h"

volatile int backup_state = 0;
volatile uint32_t preinit_cnt = 0;
volatile int alarmed = 0;
volatile int wait_time = 5;
extern uint32_t SystemCoreClock;
volatile int buttonPressed = 0;

void turnOnConsole(void);
void switchToHIRC96M(void);
void setupRTC(void);

#ifdef _GNUC_

extern void Backup_Init(void);
extern void Reset_Handler(void);
extern uint32_t __load_data;
extern uint32_t ConsoleUart;

void BackupRestore_Handler(uint32_t r0, uint32_t r1) {
    LED_On(1);
    SIMO_setVregO_C(1000); // Restore VCOREA to 1.0v
    SIMO_setVregO_B(1000); // Restore VCOREB to 1.0v
    MXC_PWRSEQ->lpcn &= ~0x3FE00000; // Disable voltage Monitors for unused rails
    LP_SIMOVregBPowerUp();

    // Check for magic values, this means 
    // ROM resumed from Backup mode
    if((r0 == 0x10000379) && (r1 == 0x4000684c)) {
        backup_state = 1;
        *(uint32_t *)0x40006830 |= 0x00010000;
    }

    // Delay until VCOREB is greater than or equal to VCOREA
    for(volatile int i = 0; i < 1000000; i++);

    MXC_SETFIELD(MXC_MCR->ctrl,MXC_F_MCR_CTRL_VDDCSW,1);

    turnOnConsole();

    if(backup_state) {
        printf("Backup Restore Detected\n");
        backup_state = 0;
    }
    printf("Code Returned to Backup Restore Handler\n");
    while(1) {
        LED_Off(1);
        TMR_Delay(MXC_TMR0, MSEC(500), NULL);
        LED_On(1);
        TMR_Delay(MXC_TMR0, MSEC(500), NULL);
    }
}
#endif // _GNUC_

void alarmHandler(void)
{
    int flags = RTC_GetFlags();
    alarmed = 1;

    if (flags & MXC_F_RTC_CTRL_ALSF) {
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALSF);
    }

    if (flags & MXC_F_RTC_CTRL_ALDF) {
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALDF);
    }
}

// *****************************************************************************
void setTrigger(int waitForTrigger)
{
    alarmed = 0;
    while (RTC_Init(MXC_RTC, 0, 0, NULL) == E_BUSY);
    while(RTC_SetTimeofdayAlarm(MXC_RTC, wait_time) == E_BUSY);
    while(RTC_EnableRTCE(MXC_RTC) == E_BUSY);
    if (waitForTrigger) {
        while(!alarmed);
    }
}

void sleep(void) 
{
    LP_EnterSleepMode();
}

void deepsleep(void) {
    Console_Shutdown();

    // Disable Instruction Cache
    ICC_Disable();
    LP_ICacheShutdown();

    //Shutdown unused power domains
    LP_VDD3PowerDown();
    LP_VDD4PowerDown();
    LP_VDD5PowerDown();
    LP_SIMOVregDPowerDown();

    SIMO_setVregO_B(810); // Reduce VCOREB to 0.81v
    
    // Clear background mode enable, enable power failure monitor. 
    LP_PowerFailMonitorEnable();

    // Enable fast wakeup 
    LP_FastWakeupEnable();
    LP_EnterDeepSleepMode();
    SIMO_setVregO_B(1000); // Restore VCOREB to 1v

}

void backup(void) {
    //Shutdown unused power domains
    LP_SIMOVregBPowerDown();

    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_FWKM;

    ICC_Disable();
    LP_ICacheShutdown();

    LP_VDD3PowerDown();
    LP_VDD4PowerDown();
    LP_VDD5PowerDown();
    LP_SIMOVregDPowerDown();

    MXC_PWRSEQ->lpcn |= 0x3FE00000; // Disable voltage Monitors for unused rails
    MXC_PWRSEQ->lpcn &= ~(0x3); // No SRAM Retention

    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;
    SIMO_setVregO_A(1800);
    SIMO_setVregO_B(850);
    SIMO_setVregO_C(850);

    LP_EnterBackupMode(NULL);
}

void turnOffUnused(void) {

    SYS_ClockEnable(SYS_PERIPH_CLOCK_USB);
    MXC_USBHS->m31_phy_ponrst = 0;
    // Disable all peripheral clocks
    MXC_GCR->perckcn0 = 0xFFFFFFFF;
    MXC_GCR->perckcn1 = 0xFFFFFFFF;

    // Allow the USB Switch to be turned off in deepsleep and backup modes
    LP_USBSWLPDisable();

    // Shut down all unneeded power gates
    LP_ICacheXIPShutdown();
    LP_CryptoShutdown();
    LP_SysCacheShutdown();
    LP_USBFIFOShutdown();
    LP_ROMShutdown();
    LP_ROM1Shutdown();
    LP_ICache1Shutdown();
    LP_SysRam1Shutdown();
    LP_SysRam2Shutdown();
    LP_SysRam3Shutdown();
    LP_SysRam4Shutdown();
    LP_SysRam5Shutdown();
}

void turnOffConsole(void) {
    MXC_GCR->perckcn0 &= ~MXC_F_GCR_PERCKCN0_T0D;
    TMR_Delay(MXC_TMR0, MSEC(100), NULL);

    Console_Shutdown();

    MXC_GCR->perckcn0 |= MXC_F_GCR_PERCKCN0_T0D | MXC_F_GCR_PERCKCN0_UART0D;
}

void turnOnConsole(void) {
    MXC_GCR->perckcn0 &= ~MXC_F_GCR_PERCKCN0_UART0D;
    Console_Init();
}

void switchToHIRC(void) {
    MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
    MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV4;
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;

    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    
    // Disable unused clocks
    while(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY)); // Wait for the switch to occur
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);

    SystemCoreClockUpdate();
}

void setupRTC(void) {
    //Set oscillator in 'low power' (aka not noise immune) mode

    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    MXC_RTC->ctrl |= MXC_F_RTC_CTRL_WE;  
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    uint32_t ctrl_temp = MXC_RTC->ctrl;
    ctrl_temp &= ~MXC_F_RTC_CTRL_X32KMD;
    ctrl_temp |= MXC_S_RTC_CTRL_X32KMD_QUIETMODE;
    MXC_RTC->ctrl = ctrl_temp;
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    MXC_RTC->ctrl &= ~MXC_F_RTC_CTRL_WE;  
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }    
}

// *****************************************************************************
int main(void)
{
    printf("**** Low Power Example ****\n");
    printf("This example will cycle through\n");
    printf("low power modes (Active, Sleep, \n");
    printf("Deepsleep, and Backup) using the RTC\n");
    printf("as a wakeup source.\n");
#ifdef __CC_ARM 
    printf("Disconnect Debbuger then reset code\n");
    printf("otherwise debbuger will interfere.\n");
#endif
    // Don't remove these lines
    printf("Starting test in 5 seconds\n");
    LP_EnableRTCAlarmWakeup();
    NVIC_SetVector(RTC_IRQn, alarmHandler);
    setTrigger(1);

// IAR will use the default entry point (reset handler)
#ifdef _GNUC_ 
    // Setup code entry point when restoring from backup mode
    // This defaults to the reset handler
    MXC_PWRSEQ->gp0 = (uint32_t)(&Backup_Init) | 1;

    // C runtime entry point when restoring from backup, using Backup_Init
    MXC_PWRSEQ->gp1 = (uint32_t)(&BackupRestore_Handler) | 1;
#endif

    printf("Turning off unused peripherals\n");
    LED_Off(0);
    LED_Off(1);
    turnOffConsole();

    turnOffUnused();
    setTrigger(1);
    setTrigger(0);
    
    turnOnConsole();
    printf("Entering Sleep Mode\n");
    turnOffConsole();
    
    sleep();
    setTrigger(0);
    
    switchToHIRC();
    turnOnConsole();
    printf("Entering Deepsleep Mode\n");
    turnOffConsole();
    
    deepsleep();
    setTrigger(0);

    turnOnConsole();
    setupRTC();
    printf("Entering Backup Mode\n");
    turnOffConsole();

    backup();
    
    printf("Returned to main()\n");
    while(1);
}

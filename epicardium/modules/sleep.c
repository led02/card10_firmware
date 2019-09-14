#include "epicardium.h"
#include "modules/modules.h"
#include "modules/log.h"

#include "card10.h"
#include "simo.h"
#include "lp.h"
#include "max86150.h"
#include "MAX77650-Arduino-Library.h"

#include "max32665.h"
#include "mxc_sys.h"
#include "mxc_pins.h"

#include <stdint.h>

/* Most code is taken and adapted rom EvKitExamples/LP/main.c */

static uint32_t old_clkcn;
static uint32_t old_perckcn0, old_perckcn1;

void GPIOWAKE_IRQHandler(void)
{
	/* Nothing to do here */
}

static void switchToHIRC(void)
{
	MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
	MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV4;
	MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;

	MXC_SETFIELD(
		MXC_GCR->clkcn,
		MXC_F_GCR_CLKCN_CLKSEL,
		MXC_S_GCR_CLKCN_CLKSEL_HIRC
	);

	while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY))
		; /* Wait for the clock switch to occur */

	/* Disable the now unused 96 MHz clock */
	MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);

	SystemCoreClockUpdate();
}

static void deepsleep(void)
{
	SIMO_setVregO_B(810); /* Reduce VCOREB to 0.81 V */
	LP_FastWakeupEnable();
	LP_EnterDeepSleepMode();
	SIMO_setVregO_B(1000); /* Restore VCOREB to 1 V */
}

static void turnOffClocks(void)
{
	old_perckcn0 = MXC_GCR->perckcn0;
	old_perckcn1 = MXC_GCR->perckcn1;

	/* Allow the USB Switch to be turned off in deepsleep and backup modes. */
	/* TODO: should this be the default setting? */
	LP_USBSWLPDisable();

	/* Disable all peripheral clocks except GPIO0 (needed for interrupt wakeup) */
	MXC_GCR->perckcn0 = 0xFFFFFFFE;
	MXC_GCR->perckcn1 = 0xFFFFFFFF;
}

static void turnOnClocks(void)
{
	MXC_GCR->perckcn0 = old_perckcn0;
	MXC_GCR->perckcn1 = old_perckcn1;
}

/*
 * Move most GPIOs into a special low power state with the fact
 * in mind that the external 3.3 V are switched off.
 *
 * E.g. this means that the SD card pins need to be pulled low
 * to preven them from backfeeding into the 3.3 V rail via their
 * external pull-up resistors.
 *
 * Pins needed to talk to the PMIC are left untouched.
 * ECG AOUT and 32 kHz out as well.
 */
static void gpio_low_power(void)
{
	/* clang-format off */
	const gpio_cfg_t pins_low_power[] = {
		{ PORT_0, PIN_0, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // FLash
		{ PORT_0, PIN_1, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // Flash
		{ PORT_0, PIN_2, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // Flash
		{ PORT_0, PIN_3, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // Flash
		{ PORT_0, PIN_4, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // Flash
		{ PORT_0, PIN_5, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // Flash
		{ PORT_0, PIN_6, GPIO_FUNC_OUT, GPIO_PAD_NONE },        // I2C 3.3V
		{ PORT_0, PIN_7, GPIO_FUNC_OUT, GPIO_PAD_NONE },        // I2C 3.3V
		{ PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE },        // Motor
		{ PORT_0, PIN_9, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },      // UART TX
		{ PORT_0, PIN_10, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },     // UART RX
		{ PORT_0, PIN_11, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // BMA400 interrupt
		// 0, 12: PMIC IRQ
		{ PORT_0, PIN_13, GPIO_FUNC_IN, GPIO_PAD_NONE },        // BHI160 interrupt, not sure if PP
		// 0, 14: PMIC I2C
		// 0, 15: PMIC I2C
		{ PORT_0, PIN_16, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },     // PMIC AMUX
		{ PORT_0, PIN_17, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // SOA GPIO
		// 0, 18: ECG AOUT
		// 0, 19: 32 kHz
		{ PORT_0, PIN_20, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // Wristband 1
		{ PORT_0, PIN_21, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // Wristband 2
		{ PORT_0, PIN_22, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // Wristband 3
		{ PORT_0, PIN_23, GPIO_FUNC_OUT, GPIO_PAD_NONE },       // IR-LED
		{ PORT_0, PIN_24, GPIO_FUNC_OUT, GPIO_PAD_NONE },       // display SS
		{ PORT_0, PIN_25, GPIO_FUNC_OUT, GPIO_PAD_NONE },       // display MOSI
		{ PORT_0, PIN_26, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // SOA GPIO
		{ PORT_0, PIN_27, GPIO_FUNC_OUT, GPIO_PAD_NONE },       // display SCK
		{ PORT_0, PIN_28, GPIO_FUNC_OUT, GPIO_PAD_NONE },       // Backlight
		{ PORT_0, PIN_29, GPIO_FUNC_IN, GPIO_PAD_PULL_DOWN },   // Wristband 4
		// 0, 30: PMIC power hold
		{ PORT_0, PIN_31, GPIO_FUNC_OUT, GPIO_PAD_NONE },   // ECG switch

		{ PORT_1, PIN_0, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_1, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_2, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_3, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_4, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_5, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // SDHC
		{ PORT_1, PIN_6, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // display RS
		{ PORT_1, PIN_7, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },  // Portexpander interrupt
		{ PORT_1, PIN_8, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },  // ECG CS TODO: better OUT high
		{ PORT_1, PIN_9, GPIO_FUNC_OUT, GPIO_PAD_NONE },    // ECG SDI
		{ PORT_1, PIN_10, GPIO_FUNC_IN, GPIO_PAD_PULL_UP }, // ECG SDO
		{ PORT_1, PIN_11, GPIO_FUNC_IN, GPIO_PAD_PULL_UP }, // ECG SCK
		{ PORT_1, PIN_11, GPIO_FUNC_IN, GPIO_PAD_PULL_UP }, // ECG INT
		{ PORT_1, PIN_13, GPIO_FUNC_IN, GPIO_PAD_NONE },    // PPG Interrupt
		{ PORT_1, PIN_14, GPIO_FUNC_OUT, GPIO_PAD_NONE },   // RGB LEDs
		{ PORT_1, PIN_15, GPIO_FUNC_OUT, GPIO_PAD_NONE },   // RGB LEDs
	};
	/* clang-format on */

	const unsigned int num_pins =
		(sizeof(pins_low_power) / sizeof(gpio_cfg_t));
	int i;
	for (i = 0; i < num_pins; i++) {
		GPIO_OutClr(&pins_low_power[i]);
		GPIO_Config(&pins_low_power[i]);
	}
}

/*
 * Go to deepsleep, turning off peripherals.
 *
 * This functions returns after waking up again.
 * Currently the only wakeup source is an interrupt
 * from the PMIC.
 *
 * Some peripherals and GPIOs are moved into a low
 * power state before going to sleep. There is no guarantee
 * that all peripherals will be moved to a low power state
 *
 * TODO: Move BHI160, BMA400, BME680, MAX30001 into a low
 * power state.
 *
 * The state of the GPIOs and generally all peripherials
 * on board is not restored after a wakeup.
 */
void sleep_deepsleep(void)
{
	LOG_WARN("pmic", "Powersave");
	epic_disp_backlight(0);
	epic_leds_set_rocket(0, 0);
	epic_leds_set_rocket(1, 0);
	epic_leds_set_rocket(2, 0);
#if 1
	/* This will fail if there is no
     * harmonic board attached */
	max86150_begin();
	max86150_getINT1();
	max86150_getINT2();
	max86150_shutDown();
#endif
	MAX77650_setEN_SBB2(0b100);
	MAX77650_setSBIA_LPM(true);
	core1_stop();
	MAX77650_getINT_GLBL();
	gpio_low_power();
	turnOffClocks();
	old_clkcn = MXC_GCR->clkcn;
	switchToHIRC();
	deepsleep();

	/* Now wait for an interrupt to wake us up */

	turnOnClocks();
	MXC_GCR->clkcn = old_clkcn;
	while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY))
		; /* Wait for the clock switch to occur */
	SystemCoreClockUpdate();
	MAX77650_setEN_SBB2(0b110);
}

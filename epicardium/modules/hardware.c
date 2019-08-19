#include "epicardium.h"

#include "api/dispatcher.h"
#include "api/interrupt-sender.h"
#include "cdcacm.h"
#include "modules/filesystem.h"
#include "modules/log.h"
#include "modules/modules.h"
#include "modules/stream.h"

#include "card10.h"

/*
 * Early init is called at the very beginning and is meant for modules which
 * absolutely need to start as soon as possible.  hardware_early_init() blocks
 * which means code in here should be fast.
 */
int hardware_early_init(void)
{
	card10_init();

#ifdef CARD10_DEBUG_CORE1
	/*
	 * The SAO pins can be reconfigured for SWCLK2 and SWDIO2 which allows
	 * debugging core 1.  This feature can optionally be enabled at
	 * compile-time.
	 */
	LOG_WARN("init", "Core 1 Debugger Mode");
	static const gpio_cfg_t swclk = {
		PORT_0,
		PIN_7,
		GPIO_FUNC_ALT3,
		GPIO_PAD_NONE,
	};
	static const gpio_cfg_t swdio = {
		PORT_0,
		PIN_6,
		GPIO_FUNC_ALT3,
		GPIO_PAD_NONE,
	};

	GPIO_Config(&swclk);
	GPIO_Config(&swdio);
#endif /* CARD10_DEBUG_CORE1 */

	/*
	 * Enable SEV-ON-PEND which is needed for proper working of the FreeRTOS
	 * tickless idle sleep in Epicardium.
	 */
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

	/*
	 * USB-Serial
	 */
	if (cdcacm_init() < 0) {
		LOG_ERR("init", "USB-Serial unavailable");
	}

	/*
	 * Flash & FatFS
	 */
	fatfs_init();

	/*
	 * API Dispatcher & API Interrupts
	 */
	api_interrupt_init();
	api_dispatcher_init();

	/*
	 * Sensor streams
	 */
	stream_init();

	return 0;
}

/*
 * hardware_init() is called after the core has been bootstrapped and is meant
 * for less critical initialization steps.  Modules which initialize here should
 * be robust against a l0dable using their API before initialization is done.
 *
 * Ideally, acquire a lock in hardware_early_init() and release it in
 * hardware_init() once initialization is done.
 */
int hardware_init(void)
{
	/* Light Sensor */
	LOG_INFO("init", "Starting light sensor ...");
	epic_light_sensor_run();

	return 0;
}

/*
 * hardware_reset() is called whenever a new l0dable is started.  hardware_reset()
 * should bring all peripherals back into a known initial state.  This does not
 * necessarily mean resetting the peripheral entirely but hardware_reset()
 * should at least bring the API facing part of a peripheral back into the state
 * a fresh booted l0dable expects.
 */
int hardware_reset(void)
{
	card10_init();
	return 0;
}

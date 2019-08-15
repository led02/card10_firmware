#include "modules/modules.h"

#include "card10.h"

/*
 * Early init is called at the very beginning and is meant for modules which
 * absolutely need to start as soon as possible.  hardware_early_init() blocks
 * which means code in here should be fast.
 */
int hardware_early_init(void)
{
	card10_init();
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

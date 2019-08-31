#include "epicardium.h"
#include "gpio.h"
#include "max32665.h"
#include "mxc_sys.h"
#include "adc.h"
#include "mxc_errors.h"
#include "modules/log.h"
#include "modules/modules.h"

/*
 * Despite what the schematic (currently, 2019-08-18) says these are the correct
 * pins for wristband GPIO 1-4 (not 0-3 as the schematic states)
 */
gpio_cfg_t gpio_configs[] = {
	[EPIC_GPIO_WRISTBAND_1] = { PORT_0,
				    PIN_21,
				    GPIO_FUNC_OUT,
				    GPIO_PAD_NONE },
	[EPIC_GPIO_WRISTBAND_2] = { PORT_0,
				    PIN_22,
				    GPIO_FUNC_OUT,
				    GPIO_PAD_NONE },
	[EPIC_GPIO_WRISTBAND_3] = { PORT_0,
				    PIN_29,
				    GPIO_FUNC_OUT,
				    GPIO_PAD_NONE },
	[EPIC_GPIO_WRISTBAND_4] = { PORT_0,
				    PIN_20,
				    GPIO_FUNC_OUT,
				    GPIO_PAD_NONE },
};

static int s_adc_channels[] = {
	[EPIC_GPIO_WRISTBAND_1] = ADC_CH_5,
	[EPIC_GPIO_WRISTBAND_2] = ADC_CH_6,
	/* on P0.29, there is no ADC available
	 * see GPIO matrix in MAX32665-MAX32668.pdf,
	 * pages 32,33
	 */
	[EPIC_GPIO_WRISTBAND_3] = -1,
	[EPIC_GPIO_WRISTBAND_4] = ADC_CH_4,
};

int epic_gpio_set_pin_mode(uint8_t pin, uint8_t mode)
{
	if (pin < EPIC_GPIO_WRISTBAND_1 || pin > EPIC_GPIO_WRISTBAND_4)
		return -EINVAL;

	gpio_cfg_t *cfg = &gpio_configs[pin];

	if (mode & EPIC_GPIO_MODE_IN) {
		cfg->func = GPIO_FUNC_IN;
		if (mode & EPIC_GPIO_MODE_OUT) {
			return -EINVAL;
		}
	} else if (mode & EPIC_GPIO_MODE_OUT) {
		cfg->func = GPIO_FUNC_OUT;
		if (mode & EPIC_GPIO_MODE_IN) {
			return -EINVAL;
		}
	} else if (mode & EPIC_GPIO_MODE_ADC) {
		if (s_adc_channels[pin] == -1) {
			LOG_WARN("gpio", "ADC not available on pin %d", pin);
			return -EINVAL;
		}
		cfg->func = GPIO_FUNC_ALT1;
		if (mode & EPIC_GPIO_MODE_OUT) {
			return -EINVAL;
		}
	} else {
		return -EINVAL;
	}

	if (!(mode & EPIC_GPIO_MODE_ADC)) {
		if (mode & EPIC_GPIO_PULL_UP) {
			cfg->pad = GPIO_PAD_PULL_UP;
		} else if (mode & EPIC_GPIO_PULL_DOWN) {
			cfg->pad = GPIO_PAD_PULL_DOWN;
		} else {
			cfg->pad = GPIO_PAD_NONE;
		}
	} else {
		cfg->pad = GPIO_PAD_NONE;
	}

	if (GPIO_Config(cfg) != E_NO_ERROR)
		return -EINVAL;
	return 0;
}

int epic_gpio_get_pin_mode(uint8_t pin)
{
	if (pin < EPIC_GPIO_WRISTBAND_1 || pin > EPIC_GPIO_WRISTBAND_4)
		return -EINVAL;

	gpio_cfg_t *cfg = &gpio_configs[pin];
	int res         = 0;
	if (cfg->func == GPIO_FUNC_IN)
		res |= EPIC_GPIO_MODE_IN;
	else if (cfg->func == GPIO_FUNC_OUT)
		res |= EPIC_GPIO_MODE_OUT;
	else if (cfg->func == GPIO_FUNC_ALT1)
		res |= EPIC_GPIO_MODE_ADC;
	if (cfg->pad == GPIO_PAD_PULL_UP)
		res |= EPIC_GPIO_PULL_UP;
	else if (cfg->pad == GPIO_PAD_PULL_DOWN)
		res |= EPIC_GPIO_PULL_DOWN;

	return res;
}

int epic_gpio_write_pin(uint8_t pin, bool on)
{
	if (pin < EPIC_GPIO_WRISTBAND_1 || pin > EPIC_GPIO_WRISTBAND_4)
		return -EINVAL;

	gpio_cfg_t *cfg = &gpio_configs[pin];
	if (cfg->func == GPIO_FUNC_IN)
		return -EINVAL;

	if (on)
		GPIO_OutSet(cfg);
	else
		GPIO_OutClr(cfg);

	return 0;
}

int epic_gpio_read_pin(uint8_t pin)
{
	if (pin < EPIC_GPIO_WRISTBAND_1 || pin > EPIC_GPIO_WRISTBAND_4)
		return -EINVAL;

	gpio_cfg_t *cfg = &gpio_configs[pin];
	if (cfg->func == GPIO_FUNC_OUT) {
		return GPIO_OutGet(cfg) != 0;
	} else if (cfg->func == GPIO_FUNC_IN) {
		return GPIO_InGet(cfg) != 0;
	} else if (cfg->func == GPIO_FUNC_ALT1) {
		int rc = hwlock_acquire(HWLOCK_ADC, pdMS_TO_TICKS(10));
		if (!rc) {
			ADC_StartConvert(s_adc_channels[pin], 0, 0);
			uint16_t value;
			int rc = ADC_GetData(&value);
			hwlock_release(HWLOCK_ADC);
			if (rc < 0) {
				return -EIO;
			}
			return (int)value;
		}
		return rc;
	} else {
		return -EINVAL;
	}
}

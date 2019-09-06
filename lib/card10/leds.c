#include "gpio.h"
#include "portexpander.h"
#include "max32665.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#define NUM_LEDS 15
#define DEFAULT_DIM_TOP 1
#define DEFAULT_DIM_BOTTOM 8
#define MAX_DIM 8

static const gpio_cfg_t rgb_dat_pin = {
	PORT_1, PIN_14, GPIO_FUNC_OUT, GPIO_PAD_NONE
};
static const gpio_cfg_t rgb_clk_pin = {
	PORT_1, PIN_15, GPIO_FUNC_OUT, GPIO_PAD_NONE
};
static uint8_t leds[NUM_LEDS][3];
static uint8_t gamma_table[3][256];
static uint8_t active_groups;
static uint8_t bottom_dim; //index 11-14
static uint8_t top_dim;    //index 0-10
static bool powersave;
static long powerup_wait_cycles = 500;

/***** Functions *****/
// *****************************************************************************

typedef struct {
	float r; // a fraction between 0 and 1
	float g; // a fraction between 0 and 1
	float b; // a fraction between 0 and 1
} rgb;

typedef struct {
	float h; // angle in degrees
	float s; // a fraction between 0 and 1
	float v; // a fraction between 0 and 1
} hsv;

#if 0
static hsv rgb2hsv(rgb in)
{
    hsv         out;
    float      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = 0.0;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}
#endif

static rgb hsv2rgb(hsv in)
{
	float hh, p, q, t, ff;
	long i;
	rgb out;

	if (in.s <= 0.0) { // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0)
		hh = 0.0;
	hh /= 60.0;
	i  = (long)hh;
	ff = hh - i;
	p  = in.v * (1.0 - in.s);
	q  = in.v * (1.0 - (in.s * ff));
	t  = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

static void shift(uint8_t data)
{
	for (int i = 0; i < 8; i++) {
		if (data & 0x80) {
			GPIO_OutSet(&rgb_dat_pin);
		} else {
			GPIO_OutClr(&rgb_dat_pin);
		}
		GPIO_OutClr(&rgb_clk_pin);
		//TMR_Delay(MXC_TMR0, MSEC(1), 0);
		GPIO_OutSet(&rgb_clk_pin);
		//TMR_Delay(MXC_TMR0, MSEC(1), 0);
		data <<= 1;
	}
}

static void leds_start(void)
{
	shift(0x00);
	shift(0x00);
	shift(0x00);
	shift(0x00);
}

static void leds_shift(uint8_t r, uint8_t g, uint8_t b, uint8_t dim)
{
	shift(0xE0 | (dim & 0x1F));
	shift(b);
	shift(g);
	shift(r);
}

static void leds_stop(void)
{
	shift(0xFF);
	shift(0xFF);
	shift(0xFF);
	shift(0xFF);
}

static uint8_t led_to_dim_value(uint8_t led)
{
	return (led < 11) ? top_dim : bottom_dim;
}

void leds_set_dim_top(uint8_t value)
{
	top_dim = (value > MAX_DIM) ? MAX_DIM : value;
}

void leds_set_dim_bottom(uint8_t value)
{
	bottom_dim = (value > MAX_DIM) ? MAX_DIM : value;
}

void leds_prep(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	leds[led][0] = r;
	leds[led][1] = g;
	leds[led][2] = b;
}

void leds_get_rgb(uint8_t led, uint8_t *rgb)
{
	rgb[0] = leds[led][0];
	rgb[1] = leds[led][1];
	rgb[2] = leds[led][2];
}

#if 0
//don't use, is buggy
void leds_set_autodim(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    if(led==NUM_LEDS){
        leds_set(led,r,g,b);
        return;
    }
    leds[led][3] = max(r,max(g,b));
    float gain = (float)255/leds[led][3]; //might cause rounding->overflow errors might debug later idk~
    leds[led][0] = (uint8_t)(r*gain);
    leds[led][1] = (uint8_t)(g*gain);
    leds[led][2] = (uint8_t)(b*gain);
}
#endif

void leds_prep_hsv(uint8_t led, float h, float s, float v)
{
	hsv in       = { h, s, v };
	rgb out      = hsv2rgb(in);
	leds[led][0] = out.r * 255;
	leds[led][1] = out.g * 255;
	leds[led][2] = out.b * 255;
}

static bool is_led_on(uint8_t led) // scheduled to be on after next update
{
	if (!led_to_dim_value(led)) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		if (leds[led][i] != 0) {
			return true;
		}
	}
	return false;
}

static uint8_t led_to_group(uint8_t led)
{
	if (led == 14) {
		return 1;
	} else if (led >= 11) {
		return 2;
	}
	return 3;
}

static uint8_t
check_privilege(void) //returns number of hierarchical groups with power
{
	for (int i = 0; i < NUM_LEDS; i++) {
		if (is_led_on(i)) {
			return led_to_group(i);
		}
	}
	return 0;
}

static uint8_t power_pin_conversion(uint8_t group)
{
	if (group == 2) {
		return 1;
	}
	if (group == 1) {
		return 2;
	}
	return 0;
}

static void power_all(void)
{
	portexpander_out_clr(PIN_0 | PIN_1 | PIN_2);
}

void leds_update_power(void)
{
	if (!powersave) {
		return;
	}
	uint8_t new_groups =
		check_privilege(); //there must be a prettier way to do this but meh
	if (new_groups == active_groups) {
		return;
	}

	uint8_t out_val = 0;
	for (int i = new_groups; i < 3; ++i) {
		out_val |= (1 << power_pin_conversion(i));
	}

	portexpander_out_put(PIN_0 | PIN_1 | PIN_2, out_val);

	if (active_groups < new_groups) {
		for (int i = 0; i < powerup_wait_cycles; i++) {
			__NOP();
		}
	}
	active_groups = new_groups;
}

void leds_powersave(bool eco)
{
	powersave = eco;
	if (!powersave) {
		power_all();
	} else {
		leds_update_power();
	}
}

void leds_update(void)
{
	leds_start();
	for (int i = NUM_LEDS - 1; i >= 0; i--) {
		leds_shift(
			gamma_table[0][leds[i][0]],
			gamma_table[1][leds[i][1]],
			gamma_table[2][leds[i][2]],
			led_to_dim_value(i)
		);
	}
	leds_stop();
}

void leds_flashlight(bool power)
{
	portexpander_out_put(PIN_7, (power) ? 0 : 0xFF);
}

void leds_set_gamma_table(uint8_t rgb_channel, uint8_t table[256])
{
	for (int i = 0; i < 256; i++) {
		gamma_table[rgb_channel][i] = table[i];
	}
}

void leds_init(void)
{
	GPIO_Config(&rgb_clk_pin);
	GPIO_Config(&rgb_dat_pin);

	GPIO_OutSet(&rgb_clk_pin);
	GPIO_OutClr(&rgb_dat_pin);

	memset(leds, 0, sizeof(leds));

	powersave  = TRUE;
	top_dim    = DEFAULT_DIM_TOP;
	bottom_dim = DEFAULT_DIM_BOTTOM;
	for (int i = 0; i < NUM_LEDS; i++) {
		for (int j = 0; j < 3; j++) {
			leds[i][j] = 0;
		}
	}
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 3; j++) {
			int k             = (i * (1 + i) + 255) >> 8;
			gamma_table[j][i] = (k * (k + 1) + 255) >> 8;
		}
	}
	leds_update();
}

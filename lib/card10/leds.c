#include "gpio.h"

#include <stdint.h>
#include <string.h>

static const gpio_cfg_t rgb_dat_pin = {PORT_1, PIN_14, GPIO_FUNC_OUT, GPIO_PAD_NONE};
static const gpio_cfg_t rgb_clk_pin = {PORT_1, PIN_15, GPIO_FUNC_OUT, GPIO_PAD_NONE};
static uint8_t led_dim = 8;
static uint8_t leds[15][3];

/***** Functions *****/
// *****************************************************************************

typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
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
    float      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
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
    for(int i=0; i<8; i++) {
        if(data & 0x80) {
            GPIO_OutSet(&rgb_dat_pin);
        }else{
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
    shift(0x00); shift(0x00); shift(0x00); shift(0x00);
}

static void leds_shift(uint8_t r, uint8_t g, uint8_t b, uint8_t dim)
{
    shift(0xE0 | (dim & 0x1F)); shift(b); shift(g); shift(r);
}

static void leds_stop(void)
{
    shift(0xFF); shift(0xFF); shift(0xFF); shift(0xFF);
}

void leds_set_dim(uint8_t dim)
{
    led_dim = dim;
}

void leds_set(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    leds[led][0] = r;
    leds[led][1] = g;
    leds[led][2] = b;
}

void leds_set_hsv(uint8_t led, float h, float s, float v)
{
    hsv in = {h, s, v};
    rgb out = hsv2rgb(in);
    leds[led][0] = out.r * 255;
    leds[led][1] = out.g * 255;
    leds[led][2] = out.b * 255;
}

void leds_update(void)
{
    leds_start();
    for(int i=3; i>=0; i--) {
        leds_shift(leds[i][0], leds[i][1], leds[i][2], led_dim);
    }
    leds_stop();
}

void leds_init(void)
{
    GPIO_Config(&rgb_clk_pin);
    GPIO_Config(&rgb_dat_pin);

    GPIO_OutSet(&rgb_clk_pin);
    GPIO_OutClr(&rgb_dat_pin);

    memset(leds, 0, sizeof(leds));
    led_dim = 8;
    leds_update();
}



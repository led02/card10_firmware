#ifndef _EPICARDIUM_H
#define _EPICARDIUM_H

#include <stdint.h>
#include <errno.h>

#ifndef __SPHINX_DOC
/* stddef.h is not recognized by hawkmoth for some odd reason */
#include <stddef.h>
#else
typedef unsigned int size_t;
#endif /* __SPHINX_DOC */

/*
 * These definitions are required for the code-generator.  Please don't touch!
 */
#ifndef API
#define API(id, def) def
#endif
#ifndef API_ISR
#define API_ISR(id, isr) void isr(void);
#endif

/*
 * IDs for all defined API calls.  These IDs should not be needed in application
 * code on any side.
 */

/* clang-format off */
#define API_UART_WRITE         0x1
#define API_UART_READ          0x2
#define API_LEDS_SET           0x3
#define API_VIBRA_SET          0x4
#define API_VIBRA_VIBRATE      0x5
#define API_STREAM_READ        0x6
#define API_INTERRUPT_ENABLE   0x7
#define API_INTERRUPT_DISABLE  0x8

#define API_DISP_OPEN          0x10
#define API_DISP_CLOSE         0x11
#define API_DISP_PRINT         0x12
#define API_DISP_CLEAR         0x13
#define API_DISP_UPDATE        0x14
#define API_DISP_LINE          0x15
#define API_DISP_RECT          0x16
#define API_DISP_CIRC          0x17
/* clang-format on */

typedef uint32_t api_int_id_t;

/**
 * Interrupts
 * ==========
 * Next to API calls, Epicardium API also has an interrupt mechanism to serve
 * the other direction.  These interrupts can be enabled/disabled
 * (masked/unmasked) using :c:func:`epic_interrupt_enable` and
 * :c:func:`epic_interrupt_disable`.
 */

/**
 * Enable/unmask an API interrupt.
 *
 * :param int_id: The interrupt to be enabled
 */
API(API_INTERRUPT_ENABLE, int epic_interrupt_enable(api_int_id_t int_id));

/**
 * Disable/mask an API interrupt.
 *
 * :param int_id: The interrupt to be disabled
 */
API(API_INTERRUPT_DISABLE, int epic_interrupt_disable(api_int_id_t int_id));

/**
 * The following interrupts are defined:
 */

/* clang-format off */
/** Reset Handler? **TODO** */
#define EPIC_INT_RESET                  0
/** ``^C`` interrupt. See :c:func:`epic_isr_ctrl_c` for details.  */
#define EPIC_INT_CTRL_C                 1
/* Debug interrupt, please ignore */
#define EPIC_INT_BHI160_TEST            2
API_ISR(EPIC_INT_BHI160_TEST, epic_isr_bhi160_test);

/* Number of defined interrupts. */
#define EPIC_INT_NUM                    3
/* clang-format on */

API_ISR(EPIC_INT_RESET, epic_isr_reset);


/**
 * UART/Serial Interface
 * =====================
 */

/**
 * Write a string to all connected serial devices.  This includes:
 *
 * - Real UART, whose pins are mapped onto USB-C pins.  Accessible via the HW-debugger.
 * - A CDC-ACM device available via USB.
 * - Maybe, in the future, bluetooth serial?
 *
 * :param str:  String to write.  Does not necessarily have to be NULL-terminated.
 * :param length:  Amount of bytes to print.
 */
API(API_UART_WRITE, void epic_uart_write_str(const char *str, intptr_t length));

/**
 * Blocking read a single character from any connected serial device.
 * ``epic_uart_read_chr`` only returns once one byte has been read.
 *
 * .. todo::
 *
 *    This API function is currently in violation of the API rules.
 *
 * :return:  The byte.
 */
API(API_UART_READ, char epic_uart_read_chr(void));

/**
 * **Interrupt Service Routine**
 *
 * A user-defineable ISR which is triggered when a ``^C`` (``0x04``) is received
 * on any serial input device.  This function is weakly aliased to
 * :c:func:`epic_isr_default` by default.
 *
 * To enable this interrupt, you need to enable :c:data:`EPIC_INT_CTRL_C`:
 *
 * .. code-block:: cpp
 *
 *    epic_interrupt_enable(EPIC_INT_CTRL_C);
 */
API_ISR(EPIC_INT_CTRL_C, epic_isr_ctrl_c);

/**
 * LEDs
 * ====
 */

/**
 * Set one of card10's RGB LEDs to a certain color.
 *
 * :param led:  Which led to set.  0-10 are the leds on the top and 11-14 are the 4 "ambient" leds.
 * :param r:  Red component of the color.
 * :param g:  Green component of the color.
 * :param b:  Blue component of the color.
 */
API(API_LEDS_SET, void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b));

/**
 * Sensor Data Streams
 * ===================
 * A few of card10's sensors can do continuous measurements.  To allow
 * performant access to their data, the following function is made for generic
 * access to streams.
 */

/**
 * Read sensor data into a buffer.  ``epic_stream_read()`` will read as many
 * sensor samples into the provided buffer as possible and return the number of
 * samples written.  If no samples are available, ``epic_stream_read()`` will
 * return ``0`` immediately.
 *
 * ``epic_stream_read()`` expects the provided buffer to have a size which is a
 * multiple of the sample size for the given stream.  For the sample-format and
 * size, please consult the sensors documentation.
 *
 * Before reading the internal sensor sample queue, ``epic_stream_read()`` will
 * call a sensor specific *poll* function to allow the sensor driver to fetch
 * new samples from its hardware.  This should, however, never take a long
 * amount of time.
 *
 * :param int sd: Sensor Descriptor.  You get sensor descriptors as return
 *    values when activating the respective sensors.
 * :param void* buf: Buffer where sensor data should be read into.
 * :param size_t count: How many bytes to read at max.  Note that fewer bytes
 *    might be read.  In most cases, this should be ``sizeof(buf)``.
 * :return: Number of data packets read (**not** number of bytes) or a negative
 *    error value.  Possible errors:
 *
 *    - ``-ENODEV``: Sensor is not currently available.
 *    - ``-EBADF``: The given sensor descriptor is unknown.
 *    - ``-EINVAL``:  ``count`` is not a multiple of the sensor's sample size.
 *    - ``-EBUSY``: The descriptor table lock could not be acquired.
 *
 * **Example**:
 *
 * .. code-block:: cpp
 *
 *    #include "epicardium.h"
 *
 *    struct foo_measurement sensor_data[16];
 *    int foo_sd, n;
 *
 *    foo_sd = epic_foo_sensor_enable(9001);
 *
 *    while (1) {
 *            n = epic_stream_read(
 *                    foo_sd,
 *                    &sensor_data,
 *                    sizeof(sensor_data)
 *            );
 *
 *            // Print out the measured sensor samples
 *            for (int i = 0; i < n; i++) {
 *                    printf("Measured: %?\n", sensor_data[i]);
 *            }
 *    }
 */
API(API_STREAM_READ, int epic_stream_read(int sd, void *buf, size_t count));

/**
 * Misc
 * ====
 */

/**
 * Turn vibration motor on or off
 *
 * :param status: 1 to turn on, 0 to turn off.
 */
API(API_VIBRA_SET, void epic_vibra_set(int status));

/**
 * Turn vibration motor on for a given time
 *
 * :param millis: number of milliseconds to run the vibration motor.
 */
API(API_VIBRA_VIBRATE, void epic_vibra_vibrate(int millis));

/**
 * Display
 * =======
 */

/** Line-Style */
enum disp_linestyle {
  /** */
  LINESTYLE_FULL = 0,
  /** */
  LINESTYLE_DOTTED = 1
};

/** Fill-Style */
enum disp_fillstyle {
  /** */
  FILLSTYLE_EMPTY = 0,
  /** */
  FILLSTYLE_FILLED = 1
};

/**
 * Locks the display.
 *
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_OPEN, int epic_disp_open());

/**
 * Unlocks the display again.
 *
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_CLOSE, int epic_disp_close());

/**
 * Causes the changes that have been written to the framebuffer
 * to be shown on the display
 */
API(API_DISP_UPDATE, int epic_disp_update());

/**
 * Prints a string into the display framebuffer
 *
 * :param posx: x position to print to. 0 <= x <= 160
 * :param posy: y position to print to. 0 <= y <= 80
 * :param pString: string to print
 * :param fg: foreground color in rgb565
 * :param bg: background color in rgb565
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_PRINT,
    int epic_disp_print(
	    uint16_t posx,
	    uint16_t posy,
	    const char *pString,
	    uint16_t fg,
	    uint16_t bg)
    );

/**
 * Fills the whole screen with one color
 *
 * :param color: fill color in rgb565
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_CLEAR, int epic_disp_clear(uint16_t color));

/**
 * Draws a line on the display
 *
 * :param xstart: x starting position; 0 <= x <= 160
 * :param ystart: y starting position; 0 <= y <= 80
 * :param xend: x ending position; 0 <= x <= 160
 * :param yend: y ending position; 0 <= y <= 80
 * :param color: line color in rgb565
 * :param linestyle: 0 for solid, 1 for dottet (almost no visual difference)
 * :param pixelsize: thickness of the line; 1 <= pixelsize <= 8
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_LINE,
    int epic_disp_line(
	    uint16_t xstart,
	    uint16_t ystart,
	    uint16_t xend,
	    uint16_t yend,
	    uint16_t color,
	    enum disp_linestyle linestyle,
	    uint16_t pixelsize)
    );

/**
 * Draws a rectangle on the display
 *
 * :param xstart: x coordinate of top left corner; 0 <= x <= 160
 * :param ystart: y coordinate of top left corner; 0 <= y <= 80
 * :param xend: x coordinate of bottom right corner; 0 <= x <= 160
 * :param yend: y coordinate of bottom right corner; 0 <= y <= 80
 * :param color: line color in rgb565
 * :param fillstyle: 0 for empty, 1 for filled
 * :param pixelsize: thickness of the rectangle outline; 1 <= pixelsize <= 8
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_RECT,
    int epic_disp_rect(
	    uint16_t xstart,
	    uint16_t ystart,
	    uint16_t xend,
	    uint16_t yend,
	    uint16_t color,
	    enum disp_fillstyle fillstyle,
	    uint16_t pixelsize)
    );

/**
 * Draws a circle on the display
 *
 * :param x: x coordinate of the center; 0 <= x <= 160
 * :param y: y coordinate of the center; 0 <= y <= 80
 * :param rad: radius of the circle
 * :param color: fill and outline color of the circle (rgb565)
 * :param fillstyle: 0 for empty, 1 for filled
 * :param pixelsize: thickness of the circle outline; 1 <= pixelsize <= 8
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_CIRC,
    int epic_disp_circ(
	    uint16_t x,
	    uint16_t y,
	    uint16_t rad,
	    uint16_t color,
	    enum disp_fillstyle fillstyle,
	    uint16_t pixelsize)
    );

#endif /* _EPICARDIUM_H */

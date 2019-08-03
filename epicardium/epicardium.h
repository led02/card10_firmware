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
#define API_SYSTEM_EXIT             0x1 /* TODO */
#define API_SYSTEM_EXEC             0x2 /* TODO */
#define API_UART_WRITE_STR          0x3
#define API_UART_READ_CHAR          0x4
#define API_UART_READ_STR           0x5
#define API_STREAM_READ             0x6
#define API_INTERRUPT_ENABLE        0x7
#define API_INTERRUPT_DISABLE       0x8

#define API_DISP_OPEN              0x20
#define API_DISP_CLOSE             0x21
#define API_DISP_PRINT             0x22
#define API_DISP_CLEAR             0x23
#define API_DISP_UPDATE            0x24
#define API_DISP_LINE              0x25
#define API_DISP_RECT              0x26
#define API_DISP_CIRC              0x27
#define API_DISP_PIXEL             0x28
#define API_DISP_FRAMEBUFFER       0x29

#define API_FILE_OPEN              0x40
#define API_FILE_CLOSE             0x41
#define API_FILE_READ              0x42
#define API_FILE_WRITE             0x44
#define API_FILE_FLUSH             0x45
#define API_FILE_SEEK              0x46
#define API_FILE_TELL              0x47
#define API_FILE_STAT              0x48

#define API_RTC_GET_SECONDS        0x50
#define API_RTC_SCHEDULE_ALARM     0x51

#define API_LEDS_SET               0x60

#define API_VIBRA_SET              0x70
#define API_VIBRA_VIBRATE          0x71

#define API_LIGHT_SENSOR_RUN       0x80
#define API_LIGHT_SENSOR_GET       0x81
#define API_LIGHT_SENSOR_STOP      0x82
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
/** UART Receive interrupt.  See :c:func:`epic_isr_uart_rx`. */
#define EPIC_INT_UART_RX                2
/** RTC Alarm interrupt.  See :c:func:`epic_isr_rtc_alarm` */
#define EPIC_INT_RTC_ALARM              3

/* Number of defined interrupts. */
#define EPIC_INT_NUM                    4
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
API(API_UART_WRITE_STR, void epic_uart_write_str(
	const char *str,
	intptr_t length
));

/**
 * Try reading a single character from any connected serial device.
 *
 * If nothing is available, :c:func:`epic_uart_read_char` returns ``(-1)``.
 *
 * :return:  The byte or ``(-1)`` if no byte was available.
 */
API(API_UART_READ_CHAR, int epic_uart_read_char(void));

/**
 * Read as many characters as possible from the UART queue.
 *
 * :c:func:`epic_uart_read_str` will not block if no new data is available.  For
 * an example, see :c:func:`epic_isr_uart_rx`.
 *
 * :param char* buf: Buffer to be filled with incoming data.
 * :param size_t cnt: Size of ``buf``.
 * :returns: Number of bytes read.  Can be ``0`` if no data was available.
 *    Might be a negative value if an error occured.
 */
API(API_UART_READ_STR, int epic_uart_read_str(char *buf, size_t cnt));

/**
 * **Interrupt Service Routine**
 *
 * UART receive interrupt.  This interrupt is triggered whenever a new character
 * becomes available on any connected UART device.  This function is weakly
 * aliased to :c:func:`epic_isr_default` by default.
 *
 * **Example**:
 *
 * .. code-block:: cpp
 *
 *    void epic_isr_uart_rx(void)
 *    {
 *            char buffer[33];
 *            int n = epic_uart_read_str(&buffer, sizeof(buffer) - 1);
 *            buffer[n] = '\0';
 *            printf("Got: %s\n", buffer);
 *    }
 *
 *    int main(void)
 *    {
 *            epic_interrupt_enable(EPIC_INT_UART_RX);
 *
 *            while (1) {
 *                    __WFI();
 *            }
 *    }
 */
API_ISR(EPIC_INT_UART_RX, epic_isr_uart_rx);

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
 * Vibration Motor
 * ===============
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
 * The card10 has an LCD screen that can be accessed from user code.
 *
 * There are two ways to access the display:
 *
 *  - *immediate mode*, where you ask Epicardium to draw shapes and text for
 *    you.  Most functions in this subsection are related to *immediate mode*.
 *  - *framebuffer mode*, where you provide Epicardium with a memory range where
 *    you already drew graphics whichever way you wanted and Epicardium will
 *    copy them to the display.  To use *framebuffer mode*, use the
 *    :c:func:`epic_disp_framebuffer` function.
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

/** Width of display in pixels */
#define DISP_WIDTH 160

/** Height of display in pixels */
#define DISP_HEIGHT 80

/**
 * Framebuffer
 *
 * The frambuffer stores pixels as RGB565, but byte swapped.  That is, for every ``(x, y)`` coordinate, there are two ``uint8_t``\ s storing 16 bits of pixel data.
 *
 * .. todo::
 *
 *    Document (x, y) in relation to chirality.
 *
 * **Example**: Fill framebuffer with red
 *
 * .. code-block:: cpp
 *
 * 	union disp_framebuffer fb;
 * 	uint16_t red = 0b1111100000000000;
 * 	for (int y = 0; y < DISP_HEIGHT; y++) {
 * 		for (int x = 0; x < DISP_WIDTH; x++) {
 * 			fb.fb[y][x][0] = red >> 8;
 * 			fb.fb[y][x][1] = red & 0xFF;
 * 		}
 * 	}
 * 	epic_disp_framebuffer(&fb);
 */
union disp_framebuffer {
  /** Coordinate based access (as shown in the example above). */
  uint8_t fb[DISP_HEIGHT][DISP_WIDTH][2];
  /** Raw byte-indexed access. */
  uint8_t raw[DISP_HEIGHT*DISP_WIDTH*2];
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
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
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
 * Draws a pixel on the display
 *
 * :param x: x position; 0 <= x <= 160
 * :param y: y position; 0 <= y <= 80
 * :param color: pixel color in rgb565
 * :return: ``0`` on success or a negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_PIXEL,
    int epic_disp_pixel(
	    uint16_t x,
	    uint16_t y,
	    uint16_t color)
    );

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

/**
 * Immediately send the contents of a framebuffer to the display. This overrides
 * anything drawn by immediate mode graphics and displayed using ``epic_disp_update``.
 *
 * :param fb: framebuffer to display
 * :return: ``0`` on success or negative value in case of an error:
 *
 *    - ``-EBUSY``: Display was already locked from another task.
 */
API(API_DISP_FRAMEBUFFER, int epic_disp_framebuffer(union disp_framebuffer *fb));


/**
 * Start continuous readout of the light sensor. Will read light level
 * at preconfigured interval and make it available via `epic_light_sensor_get()`.
 *
 * If the continuous readout was already running, this function will silently pass.
 *
 *
 * :return: `0` if the start was successful or a negative error value
 *      if an error occured. Possible errors:
 *
 *      - ``-EBUSY``: The timer could not be scheduled.
 */
API(API_LIGHT_SENSOR_RUN, int epic_light_sensor_run());

/**
 * Get the last light level measured by the continuous readout.
 *
 * :param uint16_t* value: where the last light level should be written.
 * :return: `0` if the readout was successful or a negative error
 *      value. Possible errors:
 *
 *      - ``-ENODATA``: Continuous readout not currently running.
 */
API(API_LIGHT_SENSOR_GET, int epic_light_sensor_get(uint16_t* value));


/**
 * Stop continuous readout of the light sensor.
 *
 * If the continuous readout wasn't running, this function will silently pass.
 *
 * :return: `0` if the stop was sucessful or a negative error value
 *      if an error occured. Possible errors:
 *
 *      - ``-EBUSY``: The timer stop could not be scheduled.
 */
API(API_LIGHT_SENSOR_STOP, int epic_light_sensor_stop());

/**
 * File
 * ====
 * Except for :c:func:`epic_file_open`, which models C stdio's ``fopen``
 * function, ``close``, ``read`` and ``write`` model `close(2)`_, `read(2)`_ and
 * `write(2)`_.  All file-related functions return >= ``0`` on success and
 * ``-Exyz`` on failure, with error codes from errno.h (``EIO``, ``EINVAL``
 * etc.)
 *
 * .. _close(2): http://man7.org/linux/man-pages/man2/close.2.html
 * .. _read(2): http://man7.org/linux/man-pages/man2/read.2.html
 * .. _write(2): http://man7.org/linux/man-pages/man2/write.2.html
 */

/** */
API(
	API_FILE_OPEN,
	int epic_file_open(const char* filename, const char* modeString)
);

/** */
API(API_FILE_CLOSE, int epic_file_close(int fd));

/** */
API(API_FILE_READ, int epic_file_read(int fd, void* buf, size_t nbytes));

/** */
API(
	API_FILE_WRITE,
	int epic_file_write(int fd, const void* buf, size_t nbytes)
);

/** */
API(API_FILE_FLUSH, int epic_file_flush(int fd));

/** */
API(API_FILE_SEEK, int epic_file_seek(int fd, long offset, int whence));

/** */
API(API_FILE_TELL, int epic_file_tell(int fd));

/** */
enum epic_stat_type {
	/**
	 * Basically ``ENOENT``. Although :c:func:`epic_file_stat` returns an
	 * error for 'none', the type will still be set to none additionally.
	 *
	 * This is also used internally to track open FS objects, where we use
	 * ``EPICSTAT_NONE`` to mark free objects.
	 */
	EPICSTAT_NONE,
	/** normal file */
	EPICSTAT_FILE,
	/** directory */
	EPICSTAT_DIR,
};

/** */
struct epic_stat {
	/** Entity Type: file, directory or none */
	enum epic_stat_type type;

	/*
	 * Note about padding & placement of uint32_t size:
	 *
	 *   To accomodate for future expansion, we want padding at the end of
	 *   this struct. Since sizeof(enum epic_stat_type) can not be assumed
	 *   to be have a certain size, we're placing uint32_t size here so we
	 *   can be sure it will be at offset 4, and therefore the layout of the
	 *   other fields is predictable.
	 */

	/** Size in bytes. */
	uint32_t size;

	/**
	 * Which FAT volume this entity resides on.
	 *
	 * (will be needed later once we distinguish between system and user volume)
	 */
	uint8_t volume;
	uint8_t _reserved[9];
};

#ifndef __cplusplus
#if defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
_Static_assert(sizeof(struct epic_stat) == 20, "");
#endif
#endif

/**
 * stat path
 *
 * :param char* filename: path to stat
 * :param epic_stat* stat: pointer to result
 *
 * :return: ``0`` on success, negative on error
 */
API(API_FILE_STAT, int epic_file_stat(
	const char* path, struct epic_stat* stat
));

/**
 * RTC
 * ===
 */

/**
 * Read the current RTC value.
 *
 * :return: Unix time in seconds
 */
API(API_RTC_GET_SECONDS, uint32_t epic_rtc_get_seconds(void));

/**
 * Schedule the RTC alarm for the given timestamp.
 *
 * :param uint32_t timestamp: When to schedule the IRQ
 * :return: `0` on success or a negative value if an error occured. Possible
 *    errors:
 *
 *    - ``-EINVAL``: RTC is in a bad state
 */
API(API_RTC_SCHEDULE_ALARM, int epic_rtc_schedule_alarm(uint32_t timestamp));

/**
 * **Interrupt Service Routine**
 *
 * ``epic_isr_rtc_alarm()`` is called when the RTC alarm triggers.  The RTC alarm
 * can be scheduled using :c:func:`epic_rtc_schedule_alarm`.
 */
API_ISR(EPIC_INT_RTC_ALARM, epic_isr_rtc_alarm);

#endif /* _EPICARDIUM_H */

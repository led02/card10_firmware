#include "bootloader.h"
#include "card10-version.h"

#include "card10.h"
#include "led.h"
#include "pb.h"
#include "pmic.h"

#include "board.h"
#include "crc.h"
#include "crc16-ccitt.h"
#include "ff.h"
#include "flc.h"
#include "i2c.h"
#include "icc.h"
#include "mxc_config.h"
#include "mxc_delay.h"
#include "mxc_sys.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define GPIO_PORT_IN PORT_1
#define GPIO_PIN_IN PIN_6

#define PARTITION_START (0x10000000 + 64 * 1024)
#define PARTITION_END (0x10000000 + 1024 * 1024 - 1)

DIR dir;
FATFS FatFs;

int format(void)
{
	BYTE work[FF_MAX_SS * 16];
	/* Create FAT volume */
	int res = f_mkfs("", FM_ANY | FM_SFD, 0, work, sizeof work);
	if (res != FR_OK) {
		printf("Failed to make new FS %d\n", res);
		return -1;
	}
	return 0;
}

int mount(void)
{
	FRESULT res;
	res = f_mount(&FatFs, "/", 0);
	if (res != FR_OK) {
		printf("f_mount error %d\n", res);
		return -1;
	}

	res = f_opendir(&dir, "0:");
	if (res != FR_OK) {
		printf("f_opendir error %d\n", res);
		return -1;
	}

	return 0;
}

int check_integrity(void)
{
	FIL file;
	UINT readbytes;
	char *filename = "card10.bin";
	uint8_t data[512];
	FRESULT res;

	res = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("f_open error %d\n", res);
		return -ENOENT;
	}

	uint16_t crcval = 0;
	do {
		res = f_read(&file, data, sizeof(data), &readbytes);
		if (res != FR_OK) {
			printf("f_read error %d\n", res);
			crcval = 1; // Make sure to fail the test
			break;
		}
		crcval = crc16_ccitt(crcval, data, readbytes);
	} while (readbytes == sizeof(data));

	f_close(&file);

	if (crcval != 0) {
		printf("CRC check failed. Final CRC: %d\n", crcval);
		return -EINVAL;
	}

	return 0;
}

bool is_update_needed(void)
{
	FIL file;
	UINT readbytes;
	char *filename = "card10.bin";
	uint8_t data[512];
	FRESULT res;

	res = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("f_open error %d\n", res);
		return false;
	}

	uint8_t *partition = (uint8_t *)(intptr_t)PARTITION_START;
	bool different     = false;
	do {
		res = f_read(&file, data, sizeof(data), &readbytes);
		if (res != FR_OK) {
			printf("f_read error %d\n", res);
			break; /* Going to return false, don't want to use this file */
		}
		if (memcmp(partition, data, readbytes)) {
			different = true;
			break;
		}
		partition += readbytes;
	} while (readbytes == sizeof(data));

	f_close(&file);

	return different;
}

void erase_partition(void)
{
	int ret = FLC_MultiPageErase(PARTITION_START, PARTITION_END);
	if (ret != E_NO_ERROR) {
		printf("FLC_MultiPageErase failed with %d\n", ret);
		while (1)
			;
	}
}

void flash_partition(void)
{
	FIL file;
	UINT readbytes;
	char *filename = "card10.bin";
	uint8_t data[512];
	FRESULT res;

	res = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("f_open error %d\n", res);
		while (1)
			;
	}

	uint32_t partition = PARTITION_START;

	ICC_Disable();
	do {
		res = f_read(&file, data, sizeof(data), &readbytes);
		if (res != FR_OK) {
			printf("f_read error %d\n", res);
			break; /* Going to return false, don't want to use this file */
		}
		int ret = FLC_Write(
			partition,
			readbytes,
			(uint32_t *
		)
				data); /* wild cast. not sure if this works */
		if (ret != E_NO_ERROR) {
			printf("FLC_Write failed with %d\n", ret);
			while (1)
				;
		}
		partition += readbytes;
	} while (readbytes == sizeof(data));
	ICC_Enable();

	f_close(&file);
}

static inline void boot(const void *vtable)
{
	SCB->VTOR = (uintptr_t)vtable;

	// Reset stack pointer & branch to the new reset vector.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
	__asm("mov r0, %0\n"
	      "ldr sp, [r0]\n"
	      "ldr r0, [r0, #4]\n"
	      "bx r0\n"
	      :
	      : "r"(vtable)
	      : "%sp", "r0");
#pragma GCC diagnostic pop
};

static void pmic_button(bool falling)
{
	if (falling) {
		card10_reset();
	}
}

static void msc(void)
{
	bootloader_display_header();
	bootloader_display_line(3, "USB activated.", 0xffff);
	bootloader_display_line(4, "Ready.", 0xffff);
	run_usbmsc();

	// If we return, don't try to boot. Maybe rather trigger a software reset.
	// Reason: Not sure in which state the USB peripheral is and what kind
	// of interrupts are active.
	while (1)
		;
}
/******************************************************************************/
int main(void)
{
	printf("\n\nBootloader " CARD10_VERSION "\n");
	card10_init();

	/*
	 * Make the power/reset button restart card10.
	 */
	pmic_set_button_callback(pmic_button);

	bootloader_display_init();

	// If the button is pressed, we go into MSC mode.
	if (PB_Get(3)) {
		msc();
	}

	if (mount() == 0) {
		int res = check_integrity();
		if (res == -ENOENT) {
			printf("card10.bin not found!\n");
		} else if (res == -EINVAL) {
			printf("card10.bin CRC is invalid!\n");
			bootloader_display_header();
			bootloader_display_line(
				3, "Integrity check failed", 0xffff
			);

			bootloader_display_line(4, "Trying to boot", 0xffff);
		} else if (res == 0) {
			printf("Found valid application image\n");
			if (is_update_needed()) {
				printf("Trying to update firmware from external flash\n");
				bootloader_display_header();
				bootloader_display_line(
					4, "Updating ...", 0xffff
				);
				erase_partition();
				flash_partition();
				f_unlink("card10.bin");
				bootloader_display_line(
					4, "Trying to boot", 0xffff
				);
			} else {
				printf("No update needed\n");
			}
		}

	} else {
		bootloader_display_header();
		bootloader_display_line(3, "Creating new filesystem", 0xffff);
		printf("Creating new filesystem\n");

		if (format() == 0) {
			/* Drop into MSC after a reboot */
			card10_reset();
		} else {
			bootloader_display_line(
				3, "Failed to create new filesystem", 0xffff
			);
			printf("Feiled to create new filesystem\n");
			/* Prevent bootloops */
			while (1) {
			}
		}
	}

	/* Get the intital SP of the firmware. If it is 0xFFFFFFFF, no image has been
	 * flashed yet. Drop into MSC for initial flashing. */
	if (*((uint32_t *)PARTITION_START) == 0xFFFFFFFF) {
		printf("No valid image in flash\n");
		msc();
	}

	printf("Trying to boot\n");

	boot((uintptr_t *)PARTITION_START);

	while (1) {
		// Should never be reached.
	}
}

/******************************************************************************/
void SysTick_Handler(void)
{
	mxc_delay_handler();
}

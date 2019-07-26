#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "flc.h"
#include "icc.h"
#include "i2c.h"
#include "crc.h"
#include "board.h"
#include "led.h"
#include "ff.h"
#include "crc16-ccitt.h"
#include "pb.h"
#include "display.h"
#include "GUI_Paint.h"
#include "card10.h"

#include "pmic.h"

#define GPIO_PORT_IN PORT_1
#define GPIO_PIN_IN PIN_6

#define PARTITION_START (0x10000000 + 64 * 1024)
#define PARTITION_END (0x10000000 + 1024 * 1024 - 1)

extern void run_usbmsc(void);

DIR dir;
FATFS FatFs;

bool mount(void)
{
	FRESULT res;
	res = f_mount(&FatFs, "/", 0);
	if (res != FR_OK) {
		printf("f_mount error %d\n", res);
		return false;
	}

	res = f_opendir(&dir, "0:");
	if (res != FR_OK) {
		printf("f_opendir error %d\n", res);
		return false;
	}

	return true;
}

bool check_integrity(void)
{
	FIL file;
	UINT readbytes;
	char *filename = "card10.bin";
	uint8_t data[512];
	FRESULT res;

	res = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		Paint_DrawString_EN(
			0,
			16 * 2,
			"card10.bin not found",
			&Font16,
			0x0000,
			0xffff
		);
		LCD_Update();

		printf("f_open error %d\n", res);
		return false;
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

	if (crcval == 0) {
		return true;
	} else {
		Paint_DrawString_EN(
			0,
			16 * 2,
			"Integrity check failed",
			&Font16,
			0x0000,
			0xffff
		);
		LCD_Update();
		printf("CRC check failed. Final CRC: %d\n", crcval);
		return false;
	}
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
		printf("Resetting ...\n");
		/*
		 * Give the UART fifo time to clear.
		 * TODO: Do this properly
		 */
		for (int i = 0; i < 0x1000000; i++) {
			__asm volatile("nop");
		}
		MXC_GCR->rstr0 = MXC_F_GCR_RSTR0_SYSTEM;
	}
}

/******************************************************************************/
int main(void)
{
	printf("\n\nBootloader\n");
	card10_init();

	pmic_set_button_callback(pmic_button);

	Paint_DrawString_EN(0, 16 * 0, "Bootloader", &Font16, 0x0000, 0xffff);
	Paint_DrawString_EN(0, 16 * 1, __DATE__, &Font16, 0x0000, 0xffff);

	LCD_Update();

	// If the button is pressed, we go into MSC mode.
	if (PB_Get(3)) {
		Paint_DrawString_EN(
			0,
			16 * 2,
			"USB activated. Waiting.",
			&Font16,
			0x0000,
			0xffff
		);
		LCD_Update();
		run_usbmsc();

		// If we return, don't try to boot. Maybe rather trigger a software reset.
		// Reason: Not sure in which state the USB peripheral is and what kind
		// of interrupts are active.
		while (1)
			;
	}

	if (mount()) {
		if (check_integrity()) {
			printf("Found valid application image\n");
			if (is_update_needed()) {
				printf("Trying to update application from external flash\n");
				Paint_DrawString_EN(
					0,
					16 * 4,
					"Updating...",
					&Font16,
					0x0000,
					0xffff
				);
				LCD_Update();
				erase_partition();
				flash_partition();
			} else {
				printf("No update needed\n");
			}
		} else {
			printf("Integrity check failed\n");
		}
	} else {
		Paint_DrawString_EN(
			0,
			16 * 2,
			"Failed to mount file system",
			&Font16,
			0x0000,
			0xffff
		);
		LCD_Update();
		printf("Failed to mount the external flash\n");
	}

	printf("Trying to boot\n");
	Paint_DrawString_EN(
		0, 16 * 4, "Trying to boot", &Font16, 0x0000, 0xffff
	);
	LCD_Update();
	//while(1);
	// boot partition
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

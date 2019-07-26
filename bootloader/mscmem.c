#include "mscmem.h"
#include <string.h>
#include <stdio.h>
#include "mx25lba.h"

void bootloader_dirty(void);
void bootloader_clean(void);

static int dirty = 0;

/******************************************************************************/
int mscmem_init()
{
	printf("%s\n", __func__);
	return mx25_init();
}
/******************************************************************************/
uint32_t mscmem_size(void)
{
	printf("%s\n", __func__);
	return mx25_size();
}

/******************************************************************************/
int mscmem_read(uint32_t lba, uint8_t *buffer)
{
	//printf("%s\n", __func__);
	return mx25_read(lba, buffer);
}

/******************************************************************************/
int mscmem_write(uint32_t lba, uint8_t *buffer)
{
	//printf("%s\n", __func__);
	if (dirty == 0) {
		bootloader_dirty();
	}
	dirty = 2;
	return mx25_write(lba, buffer);
}

/******************************************************************************/
int mscmem_start()
{
	printf("%s\n", __func__);
	return mx25_start();
}

/******************************************************************************/
int mscmem_stop()
{
	printf("%s\n", __func__);
	return mx25_stop();
}

/******************************************************************************/
int mscmem_ready()
{
	//printf("%s\n", __func__);
	if (dirty) {
		dirty--;
		if (dirty == 0) {
			printf("sync\n");
			mx25_sync();
			bootloader_clean();
		}
	}
	return mx25_ready();
}

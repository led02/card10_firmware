#include "mscmem.h"
#include <string.h>
#include <stdio.h>
#include "mx25lba.h"

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
int mscmem_read(uint32_t lba, uint8_t* buffer)
{
    //printf("%s\n", __func__);
	return mx25_read(lba, buffer);
}

/******************************************************************************/
int mscmem_write(uint32_t lba, uint8_t* buffer)
{
    //printf("%s\n", __func__);
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
    return mx25_ready();
}

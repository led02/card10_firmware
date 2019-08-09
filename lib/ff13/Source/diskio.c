/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"     /* FatFs lower layer API */
#include <stdbool.h>

/* Definitions of physical drive number for each drive */
#define DEV_FLASH       0   /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_SD          1   /* Example: Map MMC/SD card to physical drive 1 */

#define SDHC            0

#define SECTOR_SIZE     512UL

/*local vaiables*/
static uint8_t rtc_en;
static bool s_diskio_initialized = false;

#if SDHC
/* # of times to check for a card, should be > 1 to detect both SD and MMC */
#define INIT_CARD_RETRIES 10

/* Prototypes for module-only functions */
static DRESULT ctrl_sync(void *buff);
static DRESULT get_sector_count(void *buff);
static DRESULT get_block_size(void *buff);
static DRESULT mmc_get_csd(void *buff);

/* Globals */
unsigned int init_done = 0;
#endif

extern int mx25_init(void);
extern int mx25_start(void);
extern int mx25_stop(void);
extern uint32_t mx25_size(void);
extern int mx25_read(uint32_t lba, uint8_t* buffer);
extern int mx25_write(uint32_t lba, uint8_t* buffer);
extern int mx25_sync(void);
extern int mx25_ready(void);

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
#if 0
    #define STA_NOINIT		0x01	/* Drive not initialized */
    #define STA_NODISK		0x02	/* No medium in the drive */
    #define STA_PROTECT		0x04	/* Write protected */
#endif

	DSTATUS status = 0;
	if (!s_diskio_initialized) {
		status = STA_NOINIT | STA_NODISK;
	} else if (pdrv == 0) {
		if (mx25_ready()) {
			status = RES_OK;
		}
	}

#if SDHC
    if(pdrv == 1) {
        if (!SDHC_Card_Inserted()) {
            init_done = 0;
            status = STA_NOINIT | STA_NODISK;
        }
    }
#endif
    return status;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS status = STA_NOINIT;

	rtc_en               = 0;
	s_diskio_initialized = true;
#if (FF_FS_NORTC == 0)
    //Initialize RTC
    if (MXC_RTC->cn & MXC_F_RTC_CN_WE) {
        rtc_en = 1;
    } else {
        start_time_sec = (FF_NORTC_YEAR-1980)*SEC_IN_YEAR_AVG;
        start_time_sec += FF_NORTC_MON*SEC_IN_MONTH_AVG;
        start_time_sec += FF_NORTC_MDAY*SEC_IN_DAY;
        if(RTC_init(MXC_RTC, start_time_sec, 0) == E_NO_ERROR) {
            rtc_en = 1;
        }
    }
#endif

    if(pdrv == 0) {
        if(mx25_start() == 0) {
            status = RES_OK;
        }
    }

#if SDHC
    if(pdrv == 1) {
        if (SDHC_Card_Inserted() && (SDHC_Lib_InitCard(INIT_CARD_RETRIES) == E_NO_ERROR)) {
            /* Card initialized and ready for work */
            init_done = 1;
            status = 0;
        } else {
            status = STA_NOINIT;
        }
    }
#endif

    return status;
}

void disk_deinitialize()
{
	if (s_diskio_initialized) {
		mx25_sync();
		mx25_stop(); //XXX: or should we not?
		s_diskio_initialized = false;
	}
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT status = RES_ERROR;

    if (!s_diskio_initialized) {
        status = STA_NOINIT | STA_NODISK;
    } else if (pdrv == 0) {
        int sector_offset;
        status = RES_OK;
        for(sector_offset = 0; sector_offset < count; sector_offset++) {
            if(mx25_read(sector + sector_offset, (uint8_t*)buff + SECTOR_SIZE * sector_offset) == 1) {
                status = RES_ERROR;
                break;
            }
        }
    }
#if SDHC
    if(pdrv == 1) {
        if (SDHC_Lib_Read(buff, sector, count, SDHC_LIB_SINGLE_DATA) == E_NO_ERROR) {
            status = RES_OK;
        }
    }
#endif

    return status;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT status = RES_ERROR;

    if (!s_diskio_initialized) {
        status = STA_NOINIT | STA_NODISK;
    } else if (pdrv == 0) {
        int sector_offset;
        status = RES_OK;
        for(sector_offset = 0; sector_offset < count; sector_offset++) {
            if(mx25_write(sector + sector_offset, (uint8_t*)buff + SECTOR_SIZE * sector_offset) == 1) {
                status = RES_ERROR;
                break;
            }
        }
    }

#if SDHC
    if(pdrv == 1) {
        if (SDHC_Lib_Write(sector, (void *)buff, count, SDHC_LIB_SINGLE_DATA) != E_NO_ERROR) {
            status = RES_ERROR;
        } else {
            status = RES_OK;
        }
    }
#endif

    return status;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT status = RES_PARERR;

    if (!s_diskio_initialized) {
        status = STA_NOINIT | STA_NODISK;
    } else if (pdrv == 0) {
        switch(cmd) {
            case CTRL_SYNC:
                /* Mandatory */
                status = mx25_sync();
                break;
            case GET_SECTOR_COUNT:
                /* Mandatory */
                *((DWORD *)buff) = mx25_size() / SECTOR_SIZE;
                status = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                /* Mandatory */
                *((DWORD *)buff) = SECTOR_SIZE;
                status = RES_OK;
                break;
            default:
                status = RES_PARERR;
                break;
        }
    }

#if SDHC
    if(pdrv == 1) {
        switch(cmd) {
            case CTRL_SYNC:
                /* Mandatory */
                status = ctrl_sync(buff);
                break;
            case GET_SECTOR_COUNT:
                /* Mandatory */
                status = get_sector_count(buff);
                break;
            case GET_BLOCK_SIZE:
                /* Mandatory */
                status = get_block_size(buff);
                break;
            case MMC_GET_CSD:
                /* Optional */
                status = mmc_get_csd(buff);
                break;
            default:
                status = RES_PARERR;
                break;
        }
    }
#endif
    return status;
}

DWORD get_fattime(void) {
    if(rtc_en) {
        DWORD result;
        uint32_t seconds;
        uint8_t year, month, day, hour, minute, half_seconds;

        //Convert RTC Seconds to time
        seconds = MXC_RTC->sec + (FF_RTC_EPOCH_DELTA);
        year = seconds/SEC_IN_YEAR_AVG;    //year from epoch
        seconds = seconds%SEC_IN_YEAR_AVG; //seconds from Jan 1, $year
        month = seconds/SEC_IN_MONTH_AVG;
        seconds = seconds%SEC_IN_MONTH_AVG;
        day = seconds/SEC_IN_DAY;        //hours from 12:00am
        seconds = seconds%SEC_IN_DAY;
        hour = seconds/SEC_IN_HOUR;
        seconds = seconds%SEC_IN_HOUR;
        minute = seconds/SEC_IN_MINUTE;
        seconds = seconds%SEC_IN_MINUTE;
        half_seconds = seconds*2;

        /* Mask bits for inclusion in result */
        year &= 0x7F;
        month &= 0x0F;
        day &= 0x1F;
        hour &= 0x1F;
        minute &= 0x3F;
        half_seconds &= 0x1F;

        /* Add fields into 32bit result */
        result = year<<25;
        result |= month<<21;
        result |= day<<16;
        result |= hour<<11;
        result |= minute<<5;
        result |= half_seconds;
        return result;
    }
    else {
        return RES_NOTRDY;
    }
}

#if SDHC
static DRESULT ctrl_sync(void *buff)
{
    return RES_OK;
}

static DRESULT get_sector_count(void *buff)
{
    DRESULT status = RES_ERROR;

    mxc_sdhc_csd_regs_t csd;

    if (init_done) {
            if (SDHC_Lib_GetCSD(&csd) == E_NO_ERROR) {
            *((DWORD *)buff) = SDHC_Lib_GetCapacity(&csd) / FF_MIN_SS;
            status = RES_OK;
        }
    } else {
        status = RES_NOTRDY;
    }

    return status;
}

static DRESULT get_block_size(void *buff)
{
    DRESULT status = RES_ERROR;

    mxc_sdhc_csd_regs_t csd;
    if (init_done) {
            if (SDHC_Lib_GetCSD(&csd) == E_NO_ERROR) {
            *((DWORD *)buff) = SDHC_Lib_GetBlockSize(&csd);
            status = RES_OK;
        }
    } else {
        status = RES_NOTRDY;
    }

    return status;
}

static DRESULT mmc_get_csd(void *buff)
{
    DRESULT status = RES_ERROR;

    if (init_done) {
            if (SDHC_Lib_GetCSD(buff) == E_NO_ERROR) {
            status = RES_OK;
        }
    } else {
        status = RES_NOTRDY;
    }

    return status;
}
#endif

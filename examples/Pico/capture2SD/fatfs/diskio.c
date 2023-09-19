/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* Declarations of disk functions */
#include "MMC_SD.h"
#include "ff.h"     /* Obtains integer types */

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS
disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    return sd_init();
}

static int sdrc2dresult(int sd_rc) {
    switch (sd_rc) {
    case SD_BLOCK_DEVICE_ERROR_NONE:
        return RES_OK;
    case SD_BLOCK_DEVICE_ERROR_UNUSABLE:
    case SD_BLOCK_DEVICE_ERROR_NO_RESPONSE:
    case SD_BLOCK_DEVICE_ERROR_NO_INIT:
    case SD_BLOCK_DEVICE_ERROR_NO_DEVICE:
        return RES_NOTRDY;
    case SD_BLOCK_DEVICE_ERROR_PARAMETER:
    case SD_BLOCK_DEVICE_ERROR_UNSUPPORTED:
        return RES_PARERR;
    case SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED:
        return RES_WRPRT;
    case SD_BLOCK_DEVICE_ERROR_CRC:
    case SD_BLOCK_DEVICE_ERROR_WOULD_BLOCK:
    case SD_BLOCK_DEVICE_ERROR_ERASE:
    case SD_BLOCK_DEVICE_ERROR_WRITE:
    default:
        return RES_ERROR;
    }
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,  /* Physical drive nmuber to identify the drive */
                  BYTE *buff, /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
    int rc = sd_read_blocks(buff, sector, count);
    return sdrc2dresult(rc);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv, /* Physical drive nmuber to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   LBA_t sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
    int rc = sd_write_blocks(buff, sector, count);
    return sdrc2dresult(rc);
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
) {
    switch (cmd) {
    case GET_SECTOR_COUNT: { // Retrieves number of available sectors, the
                             // largest allowable LBA + 1, on the drive
                             // into the LBA_t variable pointed by buff.
                             // This command is used by f_mkfs and f_fdisk
                             // function to determine the size of
                             // volume/partition to be created. It is
                             // required when FF_USE_MKFS == 1.
        static LBA_t n;
        n = sd_sectors();
        *(LBA_t *)buff = n;
        if (!n)
            return RES_ERROR;
        return RES_OK;
    }
    case GET_BLOCK_SIZE: { // Retrieves erase block size of the flash
                           // memory media in unit of sector into the DWORD
                           // variable pointed by buff. The allowable value
                           // is 1 to 32768 in power of 2. Return 1 if the
                           // erase block size is unknown or non flash
                           // memory media. This command is used by only
                           // f_mkfs function and it attempts to align data
                           // area on the erase block boundary. It is
                           // required when FF_USE_MKFS == 1.
        static DWORD bs = 1;
        *(DWORD *)buff = bs;
        return RES_OK;
    }
    case GET_SECTOR_SIZE:
        *(WORD *)buff = 512;
        return RES_OK;
        break;
    case CTRL_SYNC:
        return RES_OK;
    default:
        return RES_PARERR;
    }
}

DWORD get_fattime(void) { return 0; }
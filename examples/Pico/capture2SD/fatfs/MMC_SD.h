/* sd_card.h
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

// Note: The model used here is one FatFS per SD card.
// Multiple partitions on a card are not supported.

#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ff.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SD_BLOCK_DEVICE_ERROR_NONE            0
#define SD_BLOCK_DEVICE_ERROR_WOULD_BLOCK     -5001 /*!< operation would block */
#define SD_BLOCK_DEVICE_ERROR_UNSUPPORTED     -5002 /*!< unsupported operation */
#define SD_BLOCK_DEVICE_ERROR_PARAMETER       -5003 /*!< invalid parameter */
#define SD_BLOCK_DEVICE_ERROR_NO_INIT         -5004 /*!< uninitialized */
#define SD_BLOCK_DEVICE_ERROR_NO_DEVICE       -5005 /*!< device is missing or not connected */
#define SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED -5006 /*!< write protected */
#define SD_BLOCK_DEVICE_ERROR_UNUSABLE        -5007 /*!< unusable card */
#define SD_BLOCK_DEVICE_ERROR_NO_RESPONSE     -5008 /*!< No response from device */
#define SD_BLOCK_DEVICE_ERROR_CRC             -5009 /*!< CRC error */
#define SD_BLOCK_DEVICE_ERROR_ERASE           -5010 /*!< Erase error: reset/sequence */
#define SD_BLOCK_DEVICE_ERROR_WRITE           -5011 /*!< SPI Write error: !SPI_DATA_ACCEPTED */

bool sd_init_driver();
int sd_init();
int sd_write_blocks(const uint8_t* buffer, uint64_t ulSectorNumber, uint32_t blockCnt);
int sd_read_blocks(uint8_t* buffer, uint64_t ulSectorNumber, uint32_t ulSectorCount);
// bool sd_card_detect();
uint64_t sd_sectors();

#ifdef __cplusplus
}
#endif

#endif
/* [] END OF FILE */

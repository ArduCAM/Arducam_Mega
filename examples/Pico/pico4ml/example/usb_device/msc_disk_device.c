/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "bsp/board.h"
#include "tusb.h"

#include "diskio.h"

#if CFG_TUD_MSC

// #define PICO_LOGGING_ENABLE 1

#define PICO_LOG(M, ...)                                                       \
    do {                                                                       \
        if (0)                                                                 \
            printf("%s ," M "\n", __FUNCTION__, ##__VA_ARGS__);                \
    } while (0)

static bool ejected = false;

// Some MCU doesn't have enough 8KB SRAM to store the whole disk
// We will use Flash as read-only disk with board that has
// CFG_EXAMPLE_MSC_READONLY defined

void set_msc_ready_to_attach() { ejected = false; }
// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16,
// 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8],
                        uint8_t product_id[16], uint8_t product_rev[4]) {
    (void)lun; // use same ID for both LUNs

    const char vid[] = "TinyUSB";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";

    memcpy(vendor_id, vid, strlen(vid));
    memcpy(product_id, pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void)lun;
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }
    return true;
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and
// SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size Application update
// block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count,
                         uint16_t *block_size) {
    (void)lun;

    disk_ioctl(SD_CARD, GET_SECTOR_COUNT, block_count);
    disk_ioctl(SD_CARD, GET_SECTOR_SIZE, block_size);

    // PICO_LOG("tud_msc_capacity_cb() sec_count(%lu), sec_size(%lu)",
    //          *block_count, *block_size);
}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start,
                           bool load_eject) {
    (void)lun;
    (void)power_condition;

    if (load_eject) {
        if (start) {
            ejected = false;
        } else {
            ejected = true;
        }
    }
    return true;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset,
                          void *buffer, uint32_t bufsize) {
    (void)offset;

    uint32_t count = bufsize / FLASH_SECTOR_SIZE;
    PICO_LOG("read begin! - %d", bufsize);
    disk_read(SD_CARD, buffer, lba, count);
    PICO_LOG("read complete!");

    return count * FLASH_SECTOR_SIZE;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset,
                           uint8_t *buffer, uint32_t bufsize) {
    (void)offset;

    uint32_t count = bufsize / FLASH_SECTOR_SIZE;
    PICO_LOG("write begin! - %d", bufsize);
    disk_write(SD_CARD, buffer, lba, count);
    PICO_LOG("write complete!");

    return count * FLASH_SECTOR_SIZE;
}

// Callback invoked when received an SCSI command not in built-in list below
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer,
                        uint16_t bufsize) {
    (void)buffer;
    (void)bufsize;
    int32_t resplen = 0;

    switch (scsi_cmd[0]) {
    case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        break;
    default:
        // Set Sense = Invalid Command Operation
        tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

        // negative means error -> tinyusb could stall and/or response with
        // failed status
        resplen = -1;
        break;
    }

    return resplen;
}
#endif

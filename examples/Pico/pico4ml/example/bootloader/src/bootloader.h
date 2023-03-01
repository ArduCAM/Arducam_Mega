#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct __packed __aligned(4) {
    uint32_t length;
    uint32_t crc32;
    uint8_t data[1024 * 120];
}
tFlashHeader;

extern uint32_t FLASH_IMAGE_OFFSET;
void startMainApplication(uint32_t image_offset);
uint32_t crc32(const void *data, size_t len, uint32_t crc);
uint8_t program_crc32(void);
void flashImage(tFlashHeader *header, uint32_t length);
uint8_t bin_file_crc32(uint8_t *buff);
#ifdef __cplusplus
}
#endif
#endif /*__BOOTLOADER_H*/
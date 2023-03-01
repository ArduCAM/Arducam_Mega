#include "bootloader.h"
#include "RP2040.h"
#include "hardware/flash.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/m0plus.h"
#include "hardware/structs/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/xosc.h"
#include "hardware/resets.h"
#include "hardware/dma.h"
#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "hardware/structs/dma.h"

// void* __APPLICATION_START = 0x8000;

#define bl2crc(x) (*((uint32_t *)(((uint32_t)(x) + 0xfc))))

static uint32_t sStart = XIP_BASE + 0x08000; //(uint32_t)&__APPLICATION_START;
static uint8_t sDMAChannel = 0;
uint32_t FLASH_IMAGE_OFFSET = 0x08000; //(uint32_t)&__APPLICATION_START;// sStart - XIP_BASE;
uint8_t testbuff[5] = {0x55, 0x55, 0x55, 0x55, 0x55};

// tFlashHeader upFlashData;

//****************************************************************************
// Store the given image in flash then reboot into the flashloader to replace
// the current application with the new image.
void flashImage(tFlashHeader *header, uint32_t length)
{
    // Calculate length of header plus length of data
    uint32_t totalLength = sizeof(tFlashHeader) + length;

    // Round erase length up to next 4096 byte boundary
    uint32_t eraseLength = (totalLength + 4095) & 0xfffff000;
    uint32_t status;
    status = save_and_disable_interrupts();
    flash_range_erase(FLASH_IMAGE_OFFSET, eraseLength);
    flash_range_program(FLASH_IMAGE_OFFSET, (uint8_t *)(header->data), length);
    restore_interrupts(status);
}

//****************************************************************************
// Start the main application if its boot2 image is valid.
// Will not return unless the image is invalid
void startMainApplication(uint32_t image_offset)
{
    SysTick->CTRL &= ~1;

    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICPR[0] = 0xFFFFFFFF;

    reset_block(~(
        RESETS_RESET_IO_QSPI_BITS |
        RESETS_RESET_PADS_QSPI_BITS |
        RESETS_RESET_SYSCFG_BITS |
        RESETS_RESET_PLL_SYS_BITS));

    uint32_t vtor = XIP_BASE + image_offset + 0x100;
    uint32_t reset_vector = *(volatile uint32_t *)(vtor + 0x4);
    SCB->VTOR = (volatile uint32_t)(vtor);
    asm volatile("msr msp, %0" ::"g"(*(volatile uint32_t *)vtor));
    asm volatile("bx %0" ::"r"(reset_vector));
}

//****************************************************************************
// Calculate the CRC32 (no reflection, no final XOR) of a block of data.
// This makes use of the DMA sniffer to calculate the CRC for us.  Speed is
// not really a huge issue as most of the time we just need to check the
// boot2 image is valid (252 bytes) but using DMA ought to be faster than
// looping over the data without a lookup table and is certainly a lot smaller
// than the lookup table.
uint32_t crc32(const void *data, size_t len, uint32_t crc)
{
    uint8_t dummy;

    dma_channel_config c = dma_channel_get_default_config(sDMAChannel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_sniff_enable(&c, true);

    // Turn on CRC32 (non-bit-reversed data)
    dma_sniffer_enable(sDMAChannel, 0x00, true);
    dma_hw->sniff_data = crc;

    dma_channel_configure(
        sDMAChannel,
        &c,
        &dummy,
        data,
        len,
        true // Start immediately
    );

    dma_channel_wait_for_finish_blocking(sDMAChannel);

    return (dma_hw->sniff_data);
}

uint8_t program_crc32(void)
{
    return crc32((const void *)sStart, 252, 0xffffffff) == bl2crc(sStart);
}

uint8_t bin_file_crc32(uint8_t *buff)
{
    return crc32((const void *)buff, 252, 0xffffffff) == bl2crc(buff);
}
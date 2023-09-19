/* Standard includes. */
#include "MMC_SD.h"
#include "SD_SPI_interface.h"
#include "diskio.h"
#include "pico/stdlib.h"
#include <inttypes.h>
#include <string.h>

#include "hardware/dma.h"

/* Control Tokens   */
#define SPI_DATA_RESPONSE_MASK   (0x1F)
#define SPI_DATA_ACCEPTED        (0x05)
#define SPI_DATA_CRC_ERROR       (0x0B)
#define SPI_DATA_WRITE_ERROR     (0x0D)
#define SPI_START_BLOCK          (0xFE) /*!< For Single Block Read/Write and Multiple Block Read */
#define SPI_START_BLK_MUL_WRITE  (0xFC) /*!< Start Multi-block write */
#define SPI_STOP_TRAN            (0xFD) /*!< Stop Multi-block write */

#define SPI_DATA_READ_ERROR_MASK (0xF)      /*!< Data Error Token: 4 LSB bits */
#define SPI_READ_ERROR           (0x1 << 0) /*!< Error */
#define SPI_READ_ERROR_CC        (0x1 << 1) /*!< CC Error*/
#define SPI_READ_ERROR_ECC_C     (0x1 << 2) /*!< Card ECC failed */
#define SPI_READ_ERROR_OFR       (0x1 << 3) /*!< Out of Range */

// SPI Slave Select
#define SSEL_ACTIVE              (0)
#define SSEL_INACTIVE            (1)

/** Represents the different SD/MMC card types  */
// Types
#define SDCARD_NONE              0 /**< No card is present */
#define SDCARD_V1                1 /**< v1.x Standard Capacity */
#define SDCARD_V2                2 /**< v2.x Standard capacity SD card */
#define SDCARD_V2HC              3 /**< v2.x High capacity SD card */
#define CARD_UNKNOWN             4 /**< Unknown or unsupported card */

// Only HC block size is supported. Making this a static constant reduces code
// size.
#define BLOCK_SIZE_HC            512 /*!< Block size supported for SD card is 512 bytes*/
static const uint32_t _block_size = BLOCK_SIZE_HC;

/* R1 Response Format */
#define R1_NO_RESPONSE          (0xFF)
#define R1_RESPONSE_RECV        (0x80)
#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

// Supported SD Card Commands
typedef enum {
    CMD_NOT_SUPPORTED       = -1, /**< Command not supported error */
    CMD0_GO_IDLE_STATE      = 0,  /**< Resets the SD Memory Card */
    CMD1_SEND_OP_COND       = 1,  /**< Sends host capacity support */
    CMD6_SWITCH_FUNC        = 6,  /**< Check and Switches card function */
    CMD8_SEND_IF_COND       = 8,  /**< Supply voltage info */
    CMD9_SEND_CSD           = 9,  /**< Provides Card Specific data */
    CMD10_SEND_CID          = 10, /**< Provides Card Identification */
    CMD12_STOP_TRANSMISSION = 12, /**< Forces the card to stop transmission */
    CMD13_SEND_STATUS       = 13, /**< Card responds with status */
    CMD16_SET_BLOCKLEN      = 16, /**< Length for SC card is set */
    CMD17_READ_SINGLE_BLOCK = 17, /**< Read single block of data */
    CMD18_READ_MULTIPLE_BLOCK =
        18, /**< Card transfers data blocks to host until interrupted by a STOP_TRANSMISSION command */
    CMD24_WRITE_BLOCK             = 24, /**< Write single block of data */
    CMD25_WRITE_MULTIPLE_BLOCK    = 25, /**< Continuously writes blocks of data until 'Stop Tran' token is sent */
    CMD27_PROGRAM_CSD             = 27, /**< Programming bits of CSD */
    CMD32_ERASE_WR_BLK_START_ADDR = 32, /**< Sets the address of the first write block to be erased. */
    CMD33_ERASE_WR_BLK_END_ADDR =
        33,                /**< Sets the address of the last write block of the continuous range to be erased.*/
    CMD38_ERASE      = 38, /**< Erases all previously selected write blocks */
    CMD55_APP_CMD    = 55, /**< Extend to Applications specific commands */
    CMD56_GEN_CMD    = 56, /**< General Purpose Command */
    CMD58_READ_OCR   = 58, /**< Read OCR register of card */
    CMD59_CRC_ON_OFF = 59, /**< Turns the CRC option on or off*/
    // App Commands
    ACMD6_SET_BUS_WIDTH           = 6,
    ACMD13_SD_STATUS              = 13,
    ACMD22_SEND_NUM_WR_BLOCKS     = 22,
    ACMD23_SET_WR_BLK_ERASE_COUNT = 23,
    ACMD41_SD_SEND_OP_COND        = 41,
    ACMD42_SET_CLR_CARD_DETECT    = 42,
    ACMD51_SEND_SCR               = 51,
} cmdSupported;

/* SIZE in Bytes */
#define PACKET_SIZE         6 /*!< SD Packet size CMD+ARG+CRC */
#define R1_RESPONSE_SIZE    1 /*!< Size of R1 response */
#define R2_RESPONSE_SIZE    2 /*!< Size of R2 response */
#define R3_R7_RESPONSE_SIZE 5 /*!< Size of R3/R7 response */

/* R3 Response : OCR Register */
#define OCR_HCS_CCS         (0x1 << 30)
#define OCR_LOW_VOLTAGE     (0x01 << 24)
#define OCR_3_3V            (0x1 << 20)

#define SPI_CMD(x)          (0x40 | (x & 0x3f))

#define DBG_PRINTF(M, ...)                                                                                             \
    do {                                                                                                               \
        if (0)                                                                                                         \
            printf("%s ," M "\n", __FUNCTION__, ##__VA_ARGS__);                                                        \
    } while (0)

static uint8_t sd_cmd_spi(cmdSupported cmd, uint32_t arg)
{
    uint8_t response;
    char cmdPacket[PACKET_SIZE];

    // Prepare the command packet
    cmdPacket[0] = SPI_CMD(cmd);
    cmdPacket[1] = (arg >> 24);
    cmdPacket[2] = (arg >> 16);
    cmdPacket[3] = (arg >> 8);
    cmdPacket[4] = (arg >> 0);

    switch (cmd) {
    case CMD0_GO_IDLE_STATE:
        cmdPacket[5] = 0x95;
        break;
    case CMD8_SEND_IF_COND:
        cmdPacket[5] = 0x87;
        break;
    default:
        cmdPacket[5] = 0xFF; // Make sure bit 0-End bit is high
        break;
    }
    // send a command
    for (int i = 0; i < PACKET_SIZE; i++) {
        sd_spi_write(cmdPacket[i]);
    }
    // The received byte immediataly following CMD12 is a stuff byte,
    // it should be discarded before receive the response of the CMD12.
    if (CMD12_STOP_TRANSMISSION == cmd) {
        sd_spi_write(SPI_FILL_CHAR);
    }
    // Loop for response: Response is sent back within command response time
    // (NCR), 0 to 8 bytes for SDC
    for (int i = 0; i < 0x10; i++) {
        response = sd_spi_write(SPI_FILL_CHAR);
        // Got the response
        if (!(response & R1_RESPONSE_RECV)) {
            break;
        }
    }
    return response;
}

static bool sd_wait_ready(int timeout)
{
    char resp;

    // Keep sending dummy clocks with DI held high until the card releases the
    // DO line
    absolute_time_t timeout_time = make_timeout_time_ms(timeout);
    do {
        resp = sd_spi_write(0xFF);
    } while (resp == 0x00 && 0 < absolute_time_diff_us(get_absolute_time(), timeout_time));

    if (resp == 0x00)
        DBG_PRINTF("%s failed\r\n", __FUNCTION__);

    // Return success/failure
    return (resp > 0x00);
}

static const char* cmd2str(const cmdSupported cmd)
{
    switch (cmd) {
    default:
        return "CMD_NOT_SUPPORTED";
    case CMD0_GO_IDLE_STATE:
        return "CMD0_GO_IDLE_STATE";
    case CMD1_SEND_OP_COND:
        return "CMD1_SEND_OP_COND";
    case CMD6_SWITCH_FUNC:
        return "CMD6_SWITCH_FUNC";
    case CMD8_SEND_IF_COND:
        return "CMD8_SEND_IF_COND";
    case CMD9_SEND_CSD:
        return "CMD9_SEND_CSD";
    case CMD10_SEND_CID:
        return "CMD10_SEND_CID";
    case CMD12_STOP_TRANSMISSION:
        return "CMD12_STOP_TRANSMISSION";
    case CMD13_SEND_STATUS:
        return "CMD13_SEND_STATUS or ACMD6_SET_BUS_WIDTH or "
               "ACMD13_SD_STATUS";
    case CMD16_SET_BLOCKLEN:
        return "CMD16_SET_BLOCKLEN";
    case CMD17_READ_SINGLE_BLOCK:
        return "CMD17_READ_SINGLE_BLOCK";
    case CMD18_READ_MULTIPLE_BLOCK:
        return "CMD18_READ_MULTIPLE_BLOCK";
    case CMD24_WRITE_BLOCK:
        return "CMD24_WRITE_BLOCK";
    case CMD25_WRITE_MULTIPLE_BLOCK:
        return "CMD25_WRITE_MULTIPLE_BLOCK";
    case CMD27_PROGRAM_CSD:
        return "CMD27_PROGRAM_CSD";
    case CMD32_ERASE_WR_BLK_START_ADDR:
        return "CMD32_ERASE_WR_BLK_START_ADDR";
    case CMD33_ERASE_WR_BLK_END_ADDR:
        return "CMD33_ERASE_WR_BLK_END_ADDR";
    case CMD38_ERASE:
        return "CMD38_ERASE";
    case CMD55_APP_CMD:
        return "CMD55_APP_CMD";
    case CMD56_GEN_CMD:
        return "CMD56_GEN_CMD";
    case CMD58_READ_OCR:
        return "CMD58_READ_OCR";
    case CMD59_CRC_ON_OFF:
        return "CMD59_CRC_ON_OFF";
    // case ACMD6_SET_BUS_WIDTH:
    // case ACMD13_SD_STATUS:
    case ACMD22_SEND_NUM_WR_BLOCKS:
        return "ACMD22_SEND_NUM_WR_BLOCKS";
    case ACMD23_SET_WR_BLK_ERASE_COUNT:
        return "ACMD23_SET_WR_BLK_ERASE_COUNT";
    case ACMD41_SD_SEND_OP_COND:
        return "ACMD41_SD_SEND_OP_COND";
    case ACMD42_SET_CLR_CARD_DETECT:
        return "ACMD42_SET_CLR_CARD_DETECT";
    case ACMD51_SEND_SCR:
        return "ACMD51_SEND_SCR";
    }
}

#define SD_COMMAND_TIMEOUT 2000 /*!< Timeout in ms for response */

static int card_type        = 0;
static bool use_card_detect = false;
static int m_Status         = STA_NOINIT;
static uint64_t sectors; // Assigned dynamically

static int sd_cmd(const cmdSupported cmd, uint32_t arg, bool isAcmd, uint32_t* resp)
{
    DBG_PRINTF("%s(%s(0x%08lx)): ", __FUNCTION__, cmd2str(cmd), arg);

    int32_t status = SD_BLOCK_DEVICE_ERROR_NONE;
    uint32_t response;

    // No need to wait for card to be ready when sending the stop command
    if (CMD12_STOP_TRANSMISSION != cmd) {
        if (false == sd_wait_ready(SD_COMMAND_TIMEOUT)) {
            DBG_PRINTF("%s:%d: Card not ready yet\r\n", __FILE__, __LINE__);
        }
    }
    // Re-try command
    for (int i = 0; i < 3; i++) {
        // Send CMD55 for APP command first
        if (isAcmd) {
            response = sd_cmd_spi(CMD55_APP_CMD, 0x0);
            // Wait for card to be ready after CMD55
            if (false == sd_wait_ready(SD_COMMAND_TIMEOUT)) {
                DBG_PRINTF("%s:%d: Card not ready yet\r\n", __FILE__, __LINE__);
            }
        }
        // Send command over SPI interface
        response = sd_cmd_spi(cmd, arg);
        if (R1_NO_RESPONSE == response) {
            DBG_PRINTF("No response CMD:%d\r\n", cmd);
            continue;
        }
        break;
    }
    // Pass the response to the command call if required
    if (NULL != resp) {
        *resp = response;
    }
    // Process the response R1  : Exit on CRC/Illegal command error/No response
    if (R1_NO_RESPONSE == response) {
        DBG_PRINTF("No response CMD:%d response: 0x%" PRIx32 "\r\n", cmd, response);
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE; // No device
    }
    if (response & R1_COM_CRC_ERROR && ACMD23_SET_WR_BLK_ERASE_COUNT != cmd) {
        DBG_PRINTF("CRC error CMD:%d response 0x%" PRIx32 "\r\n", cmd, response);
        return SD_BLOCK_DEVICE_ERROR_CRC; // CRC error
    }
    if (response & R1_ILLEGAL_COMMAND) {
        if (ACMD23_SET_WR_BLK_ERASE_COUNT != cmd)
            DBG_PRINTF("Illegal command CMD:%d response 0x%" PRIx32 "\r\n", cmd, response);
        if (CMD8_SEND_IF_COND == cmd) {
            // Illegal command is for Ver1 or not SD Card
            card_type = CARD_UNKNOWN;
        }
        return SD_BLOCK_DEVICE_ERROR_UNSUPPORTED; // Command not supported
    }

    //	DBG_PRINTF("CMD:%d \t arg:0x%" PRIx32 " \t Response:0x%" PRIx32 "\r\n",
    // cmd, arg, response);
    // Set status for other errors
    if ((response & R1_ERASE_RESET) || (response & R1_ERASE_SEQUENCE_ERROR)) {
        status = SD_BLOCK_DEVICE_ERROR_ERASE; // Erase error
    } else if ((response & R1_ADDRESS_ERROR) || (response & R1_PARAMETER_ERROR)) {
        // Misaligned address / invalid address block length
        status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    // Get rest of the response part for other commands
    switch (cmd) {
    case CMD8_SEND_IF_COND: // Response R7
        DBG_PRINTF("V2-Version Card\r\n");
        card_type = SDCARD_V2; // fallthrough
        // Note: No break here, need to read rest of the response
    case CMD58_READ_OCR: // Response R3
        response = (sd_spi_write(SPI_FILL_CHAR) << 24);
        response |= (sd_spi_write(SPI_FILL_CHAR) << 16);
        response |= (sd_spi_write(SPI_FILL_CHAR) << 8);
        response |= sd_spi_write(SPI_FILL_CHAR);
        DBG_PRINTF("R3/R7: 0x%" PRIx32 "\r\n", response);
        break;
    case CMD12_STOP_TRANSMISSION: // Response R1b
    case CMD38_ERASE:
        sd_wait_ready(SD_COMMAND_TIMEOUT);
        break;
    case CMD13_SEND_STATUS: // Response R2
        response <<= 8;
        response |= sd_spi_write(SPI_FILL_CHAR);
        if (response) {
            DBG_PRINTF("R2: 0x%" PRIx32 "\r\n", response);
            if (response & 0x01 << 0) {
                DBG_PRINTF("Card is Locked                         \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE;
            }
            if (response & 0x01 << 1) {
                DBG_PRINTF("WP Erase Skip, Lock/Unlock Cmd Failed  \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED;
            }
            if (response & 0x01 << 2) {
                DBG_PRINTF("Error                                  \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE;
            }
            if (response & 0x01 << 3) {
                DBG_PRINTF("CC Error                               \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE;
            }
            if (response & 0x01 << 4) {
                DBG_PRINTF("Card ECC Failed                        \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE;
            }
            if (response & 0x01 << 5) {
                DBG_PRINTF("WP Violation                           \r\n");
                status = SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED;
            }
            if (response & 0x01 << 6) {
                DBG_PRINTF("Erase Param                            \r\n");
                status = SD_BLOCK_DEVICE_ERROR_ERASE;
            }
            if (response & 0x01 << 7) {
                DBG_PRINTF("Out of Range, CSD_Overwrite            \r\n");
                status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
            }
            if (response & 0x01 << 8) {
                DBG_PRINTF("In Idle State                          \r\n");
                status = SD_BLOCK_DEVICE_ERROR_NONE;
            }
            if (response & 0x01 << 9) {
                DBG_PRINTF("Erase Reset                            \r\n");
                status = SD_BLOCK_DEVICE_ERROR_ERASE;
            }
            if (response & 0x01 << 10) {
                DBG_PRINTF("Illegal Command                        \r\n");
                status = SD_BLOCK_DEVICE_ERROR_UNSUPPORTED;
            }
            if (response & 0x01 << 11) {
                DBG_PRINTF("Com CRC Error                          \r\n");
                status = SD_BLOCK_DEVICE_ERROR_CRC;
            }
            if (response & 0x01 << 12) {
                DBG_PRINTF("Erase Sequence Error                   \r\n");
                status = SD_BLOCK_DEVICE_ERROR_ERASE;
            }
            if (response & 0x01 << 13) {
                DBG_PRINTF("Address Error                          \r\n");
                status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
            }
            if (response & 0x01 << 14) {
                DBG_PRINTF("Parameter Error                        \r\n");
                status = SD_BLOCK_DEVICE_ERROR_PARAMETER;
            }
            break;
        }
    default: // Response R1
        break;
    }
    // Pass the updated response to the command
    if (NULL != resp) {
        *resp = response;
    }
    return status;
}

// /* Return non-zero if the SD-card is present. */
// bool sd_card_detect() {
//     DBG_PRINTF("> %s\r\n", __FUNCTION__);
//     if (!use_card_detect) {
//         m_Status &= ~STA_NODISK;
//         return true;
//     }
//     /*!< Check GPIO to detect SD */
//     if (gpio_get(pSD->card_detect_gpio) == pSD->card_detected_true) {
//         // The socket is now occupied
//         m_Status &= ~STA_NODISK;
//         DBG_PRINTF("SD card detected!\r\n");
//         return true;
//     } else {
//         // The socket is now empty
//         m_Status |= (STA_NODISK | STA_NOINIT);
//         card_type = SDCARD_NONE;
//         printf("No SD card detected!\r\n");
//         return false;
//     }
// }

/*!< Number of retries for sending CMDO */
#define SD_CMD0_GO_IDLE_STATE_RETRIES 10

static uint32_t sd_go_idle_state()
{
    uint32_t response;

    /* Resetting the MCU SPI master may not reset the on-board SDCard, in which
     * case when MCU power-on occurs the SDCard will resume operations as
     * though there was no reset. In this scenario the first CMD0 will
     * not be interpreted as a command and get lost. For some cards retrying
     * the command overcomes this situation. */
    for (int i = 0; i < SD_CMD0_GO_IDLE_STATE_RETRIES; i++) {
        sd_cmd(CMD0_GO_IDLE_STATE, 0x0, false, &response);
        if (R1_IDLE_STATE == response) {
            break;
        }
        sd_spi_deselect();
        busy_wait_us(100 * 1000);
        sd_spi_select();
    }
    return response;
}

/* R7 response pattern for CMD8 */
#define CMD8_PATTERN (0xAA)

static int sd_cmd8()
{
    uint32_t arg      = (CMD8_PATTERN << 0); // [7:0]check pattern
    uint32_t response = 0;
    int32_t status    = SD_BLOCK_DEVICE_ERROR_NONE;

    arg |= (0x1 << 8); // 2.7-3.6V             // [11:8]supply voltage(VHS)

    status = sd_cmd(CMD8_SEND_IF_COND, arg, false, &response);
    // Verify voltage and pattern for V2 version of card
    if ((SD_BLOCK_DEVICE_ERROR_NONE == status) && (SDCARD_V2 == card_type)) {
        // If check pattern is not matched, CMD8 communication is not valid
        if ((response & 0xFFF) != arg) {
            DBG_PRINTF("CMD8 Pattern mismatch 0x%" PRIx32 " : 0x%" PRIx32 "\r\n", arg, response);
            card_type = CARD_UNKNOWN;
            status    = SD_BLOCK_DEVICE_ERROR_UNUSABLE;
        }
    }
    return status;
}

static uint32_t ext_bits(unsigned char* data, int msb, int lsb)
{
    uint32_t bits = 0;
    uint32_t size = 1 + msb - lsb;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t position = lsb + i;
        uint32_t byte     = 15 - (position >> 3);
        uint32_t bit      = position & 0x7;
        uint32_t value    = (data[byte] >> bit) & 1;
        bits |= value << i;
    }
    return bits;
}

static int sd_read_bytes(uint8_t* buffer, uint32_t length);

static uint64_t sd_sectors_nolock()
{
    uint32_t c_size, c_size_mult, read_bl_len;
    uint32_t block_len, mult, blocknr;
    uint32_t hc_c_size;
    uint64_t blocks = 0, capacity = 0;

    // CMD9, Response R2 (R1 byte + 16-byte block read)
    if (sd_cmd(CMD9_SEND_CSD, 0x0, false, 0) != 0x0) {
        DBG_PRINTF("Didn't get a response from the disk\r\n");
        return 0;
    }
    uint8_t csd[16];
    if (sd_read_bytes(csd, 16) != 0) {
        DBG_PRINTF("Couldn't read csd response from disk\r\n");
        return 0;
    }
    // csd_structure : csd[127:126]
    int csd_structure = ext_bits(csd, 127, 126);
    switch (csd_structure) {
    case 0:
        c_size      = ext_bits(csd, 73, 62);       // c_size        : csd[73:62]
        c_size_mult = ext_bits(csd, 49, 47);       // c_size_mult   : csd[49:47]
        read_bl_len = ext_bits(csd, 83, 80);       // read_bl_len   : csd[83:80] - the
                                                   // *maximum* read block length
        block_len = 1 << read_bl_len;              // BLOCK_LEN = 2^READ_BL_LEN
        mult      = 1 << (c_size_mult + 2);        // MULT = 2^C_SIZE_MULT+2 (C_SIZE_MULT < 8)
        blocknr   = (c_size + 1) * mult;           // BLOCKNR = (C_SIZE+1) * MULT
        capacity  = (uint64_t)blocknr * block_len; // memory capacity = BLOCKNR * BLOCK_LEN
        blocks    = capacity / _block_size;
        DBG_PRINTF("Standard Capacity: c_size: %" PRIu32 "\r\n", c_size);
        DBG_PRINTF("Sectors: 0x%llx : %llu\r\n", blocks, blocks);
        DBG_PRINTF("Capacity: 0x%llx : %llu MB\r\n", capacity, (capacity / (1024U * 1024U)));
        break;

    case 1:
        hc_c_size = ext_bits(csd, 69, 48); // device size : C_SIZE : [69:48]
        blocks    = (hc_c_size + 1) << 10; // block count = C_SIZE+1) * 1K
                                           // byte (512B is block size)
        DBG_PRINTF("SDHC/SDXC Card: hc_c_size: %" PRIu32 "\r\n", hc_c_size);
        DBG_PRINTF("Sectors: %8llu\r\n", blocks);
        DBG_PRINTF("Capacity: %8llu MB\r\n", (blocks / (2048U)));
        break;

    default:
        DBG_PRINTF("CSD struct unsupported\r\n");
        return 0;
    };
    return blocks;
}

uint64_t sd_sectors()
{
    sd_spi_select();
    uint64_t sectors = sd_sectors_nolock();
    sd_spi_deselect();
    return sectors;
}

// SPI function to wait till chip is ready and sends start token
static bool sd_wait_token(uint8_t token)
{
    DBG_PRINTF("%s(0x%02hhx)\r\n", __FUNCTION__, token);

    const uint32_t timeout       = SD_COMMAND_TIMEOUT; // Wait for start token
    absolute_time_t timeout_time = make_timeout_time_ms(timeout);
    do {
        if (token == sd_spi_write(SPI_FILL_CHAR)) {
            return true;
        }
    } while (0 < absolute_time_diff_us(get_absolute_time(), timeout_time));
    DBG_PRINTF("sd_wait_token: timeout\r\n");
    return false;
}

/*!< For Single Block Read/Write and Multiple Block Read */
#define SPI_START_BLOCK (0xFE)

static int sd_read_bytes(uint8_t* buffer, uint32_t length)
{
    uint16_t crc;

    // read until start byte (0xFE)
    if (false == sd_wait_token(SPI_START_BLOCK)) {
        DBG_PRINTF("%s:%d Read timeout\r\n", __FILE__, __LINE__);
        return SD_BLOCK_DEVICE_ERROR_NO_RESPONSE;
    }
    // read data
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = sd_spi_write(SPI_FILL_CHAR);
    }
    // Read the CRC16 checksum for the data block
    crc = (sd_spi_write(SPI_FILL_CHAR) << 8);
    crc |= sd_spi_write(SPI_FILL_CHAR);

    return 0;
}

static int sd_read_block(uint8_t* buffer, uint32_t length)
{
    uint16_t crc;

    // read until start byte (0xFE)
    if (false == sd_wait_token(SPI_START_BLOCK)) {
        DBG_PRINTF("%s:%d Read timeout\r\n", __FILE__, __LINE__);
        return SD_BLOCK_DEVICE_ERROR_NO_RESPONSE;
    }
    // read data
    // bool spi_transfer(const uint8_t *tx, uint8_t *rx, size_t length)
    if (!sd_spi_read_blocking(buffer, length)) {
        return SD_BLOCK_DEVICE_ERROR_NO_RESPONSE;
    }
    // Read the CRC16 checksum for the data block
    crc = (sd_spi_write(SPI_FILL_CHAR) << 8);
    crc |= sd_spi_write(SPI_FILL_CHAR);

    return SD_BLOCK_DEVICE_ERROR_NONE;
}

static int in_sd_read_blocks(uint8_t* buffer, uint64_t ulSectorNumber, uint32_t ulSectorCount)
{
    uint32_t blockCnt = ulSectorCount;

    if (ulSectorNumber + blockCnt > sectors)
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    if (m_Status & (STA_NOINIT | STA_NODISK))
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;

    int status = SD_BLOCK_DEVICE_ERROR_NONE;

    uint64_t addr;
    // SDSC Card (CCS=0) uses byte unit address
    // SDHC and SDXC Cards (CCS=1) use block unit address (512 Bytes unit)
    if (SDCARD_V2HC == card_type) {
        addr = ulSectorNumber;
    } else {
        addr = ulSectorNumber * _block_size;
    }
    // Write command ro receive data
    if (blockCnt > 1) {
        status = sd_cmd(CMD18_READ_MULTIPLE_BLOCK, addr, false, 0);
    } else {
        status = sd_cmd(CMD17_READ_SINGLE_BLOCK, addr, false, 0);
    }
    if (SD_BLOCK_DEVICE_ERROR_NONE != status) {
        return status;
    }
    // receive the data : one block at a time
    int rd_status = 0;
    while (blockCnt) {
        if (0 != sd_read_block(buffer, _block_size)) {
            rd_status = SD_BLOCK_DEVICE_ERROR_NO_RESPONSE;
            break;
        }
        buffer += _block_size;
        --blockCnt;
    }
    // Send CMD12(0x00000000) to stop the transmission for multi-block transfer
    if (ulSectorCount > 1) {
        status = sd_cmd(CMD12_STOP_TRANSMISSION, 0x0, false, 0);
    }
    return rd_status ? rd_status : status;
}

int sd_read_blocks(uint8_t* buffer, uint64_t ulSectorNumber, uint32_t ulSectorCount)
{
    sd_spi_select();
    DBG_PRINTF("sd_read_blocks(0x%p, 0x%llx, 0x%lx)\r\n", buffer, ulSectorNumber, ulSectorCount);
    int status = in_sd_read_blocks(buffer, ulSectorNumber, ulSectorCount);
    sd_spi_deselect();
    return status;
}
bool sd_spi_write_blocking(const uint8_t* tx, size_t length);

static uint8_t sd_write_block(const uint8_t* buffer, uint8_t token, uint32_t length)
{
    uint16_t crc     = (~0);
    uint8_t response = 0xFF;

    // indicate start of block
    sd_spi_write(token);

    // write the data
    bool ret = sd_spi_write_blocking(buffer, length);
    // bool ret = sd_spi_transfer(buffer, NULL, length);
    
    // write the checksum CRC16
    sd_spi_write(crc >> 8);
    sd_spi_write(crc);

    // check the response token
    response = sd_spi_write(SPI_FILL_CHAR);

    // Wait for last block to be written
    if (false == sd_wait_ready(SD_COMMAND_TIMEOUT)) {
        DBG_PRINTF("%s:%d: Card not ready yet\r\n", __FILE__, __LINE__);
    }
    return (response & SPI_DATA_RESPONSE_MASK);
}

/** Program blocks to a block device
 *
 *
 *  @param buffer       Buffer of data to write to blocks
 *  @param ulSectorNumber     Logical Address of block to begin writing to (LBA)
 *  @param blockCnt     Size to write in blocks
 *  @return         SD_BLOCK_DEVICE_ERROR_NONE(0) - success
 *                  SD_BLOCK_DEVICE_ERROR_NO_DEVICE - device (SD card) is
 * missing or not connected SD_BLOCK_DEVICE_ERROR_CRC - crc error
 *                  SD_BLOCK_DEVICE_ERROR_PARAMETER - invalid parameter
 *                  SD_BLOCK_DEVICE_ERROR_UNSUPPORTED - unsupported command
 *                  SD_BLOCK_DEVICE_ERROR_NO_INIT - device is not initialized
 *                  SD_BLOCK_DEVICE_ERROR_WRITE - SPI write error
 *                  SD_BLOCK_DEVICE_ERROR_ERASE - erase error
 */
static int in_sd_write_blocks(const uint8_t* buffer, uint64_t ulSectorNumber, uint32_t blockCnt)
{
    if (ulSectorNumber + blockCnt > sectors)
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    if (m_Status & (STA_NOINIT | STA_NODISK))
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;

    int status = SD_BLOCK_DEVICE_ERROR_NONE;
    uint8_t response;
    uint64_t addr;

    // SDSC Card (CCS=0) uses byte unit address
    // SDHC and SDXC Cards (CCS=1) use block unit address (512 Bytes unit)
    if (SDCARD_V2HC == card_type) {
        addr = ulSectorNumber;
    } else {
        addr = ulSectorNumber * _block_size;
    }
    // Send command to perform write operation
    if (blockCnt == 1) {
        // Single block write command
        if (SD_BLOCK_DEVICE_ERROR_NONE != (status = sd_cmd(CMD24_WRITE_BLOCK, addr, false, 0))) {
            return status;
        }
        // Write data
        response = sd_write_block(buffer, SPI_START_BLOCK, _block_size);

        // Only CRC and general write error are communicated via response token
        if (response != SPI_DATA_ACCEPTED) {
            DBG_PRINTF("Single Block Write failed: 0x%x \r\n", response);
            status = SD_BLOCK_DEVICE_ERROR_WRITE;
        }
    } else {
        // Pre-erase setting prior to multiple block write operation
        sd_cmd(ACMD23_SET_WR_BLK_ERASE_COUNT, blockCnt, 1, 0);

        // Some SD cards want to be deselected between every bus transaction:
        sd_spi_deselect_pulse();

        // Multiple block write command
        if (SD_BLOCK_DEVICE_ERROR_NONE != (status = sd_cmd(CMD25_WRITE_MULTIPLE_BLOCK, addr, false, 0))) {
            return status;
        }
        // Write the data: one block at a time
        do {
            response = sd_write_block(buffer, SPI_START_BLK_MUL_WRITE, _block_size);
            if (response != SPI_DATA_ACCEPTED) {
                DBG_PRINTF("Multiple Block Write failed: 0x%x\r\n", response);
                status = SD_BLOCK_DEVICE_ERROR_WRITE;
                break;
            }
            buffer += _block_size;
        } while (--blockCnt); // Send all blocks of data
        /* In a Multiple Block write operation, the stop transmission will be
         * done by sending 'Stop Tran' token instead of 'Start Block' token at
         * the beginning of the next block
         */
        sd_spi_write(SPI_STOP_TRAN);
    }
    uint32_t stat = 0;
    // Some SD cards want to be deselected between every bus transaction:
    sd_spi_deselect_pulse();
    status = sd_cmd(CMD13_SEND_STATUS, 0, false, &stat);
    return status;
}

int sd_write_blocks(const uint8_t* buffer, uint64_t ulSectorNumber, uint32_t blockCnt)
{
    sd_spi_select();
    DBG_PRINTF("sd_write_blocks(0x%p, 0x%llx, 0x%lx)\r\n", buffer, ulSectorNumber, blockCnt);
    int status = in_sd_write_blocks(buffer, ulSectorNumber, blockCnt);
    sd_spi_deselect();
    return status;
}

static int sd_init_medium()
{
    int32_t status = SD_BLOCK_DEVICE_ERROR_NONE;
    uint32_t response, arg;
    /*
    Power ON or card insersion
    After supply voltage reached above 2.2 volts,
    wait for one millisecond at least.
    Set SPI clock rate between 100 kHz and 400 kHz.
    Set DI and CS high and apply 74 or more clock pulses to SCLK.
    The card will enter its native operating mode and go ready to accept native
    command.
    */
    sd_spi_go_low_frequency();
    sd_spi_send_initializing_sequence();

    // The card is transitioned from SDCard mode to SPI mode by sending the CMD0
    // + CS Asserted("0")
    if (sd_go_idle_state() != R1_IDLE_STATE) {
        DBG_PRINTF("No disk, or could not put SD card in to SPI idle state\r\n");
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }

    // Send CMD8, if the card rejects the command then it's probably using the
    // legacy protocol, or is a MMC, or just flat-out broken
    status = sd_cmd8();
    if (SD_BLOCK_DEVICE_ERROR_NONE != status && SD_BLOCK_DEVICE_ERROR_UNSUPPORTED != status) {
        return status;
    }

    // Read OCR - CMD58 Response contains OCR register
    if (SD_BLOCK_DEVICE_ERROR_NONE != (status = sd_cmd(CMD58_READ_OCR, 0x0, false, &response))) {
        return status;
    }
    // Check if card supports voltage range: 3.3V
    if (!(response & OCR_3_3V)) {
        card_type = CARD_UNKNOWN;
        status    = SD_BLOCK_DEVICE_ERROR_UNUSABLE;
        return status;
    }

    // HCS is set 1 for HC/XC capacity cards for ACMD41, if supported
    arg = 0x0;
    if (SDCARD_V2 == card_type) {
        arg |= OCR_HCS_CCS;
    }

    /* Idle state bit in the R1 response of ACMD41 is used by the card to inform
     * the host if initialization of ACMD41 is completed. "1" indicates that the
     * card is still initializing. "0" indicates completion of initialization.
     * The host repeatedly issues ACMD41 until this bit is set to "0".
     */
    absolute_time_t timeout_time = make_timeout_time_ms(SD_COMMAND_TIMEOUT);
    do {
        status = sd_cmd(ACMD41_SD_SEND_OP_COND, arg, true, &response);
    } while (response & R1_IDLE_STATE && 0 < absolute_time_diff_us(get_absolute_time(), timeout_time));

    // Initialization complete: ACMD41 successful
    if ((SD_BLOCK_DEVICE_ERROR_NONE != status) || (0x00 != response)) {
        card_type = CARD_UNKNOWN;
        DBG_PRINTF("Timeout waiting for card\r\n");
        return status;
    }

    if (SDCARD_V2 == card_type) {
        // Get the card capacity CCS: CMD58
        if (SD_BLOCK_DEVICE_ERROR_NONE == (status = sd_cmd(CMD58_READ_OCR, 0x0, false, &response))) {
            // High Capacity card
            if (response & OCR_HCS_CCS) {
                card_type = SDCARD_V2HC;
                DBG_PRINTF("Card Initialized: High Capacity Card\r\n");
            } else {
                DBG_PRINTF("Card Initialized: Standard Capacity Card: Version "
                           "2.x\r\n");
            }
        }
    } else {
        card_type = SDCARD_V1;
        DBG_PRINTF("Card Initialized: Version 1.x Card\r\n");
    }

    status = sd_cmd(CMD59_CRC_ON_OFF, 0, false, 0);

    return status;
}

int sd_init()
{
    DBG_PRINTF("> %s\r\n", __FUNCTION__);
    if (!sd_init_driver()) {
        m_Status |= STA_NOINIT;
        return m_Status;
    }
    // Initialize the member variables
    card_type = SDCARD_NONE;

    sd_spi_select();

    int err = sd_init_medium();
    if (SD_BLOCK_DEVICE_ERROR_NONE != err) {
        DBG_PRINTF("Failed to initialize card\r\n");
        sd_spi_deselect();
        sd_spi_go_high_frequency();
        return m_Status;
    }
    DBG_PRINTF("SD card initialized\r\n");
    sectors = sd_sectors_nolock();
    if (0 == sectors) {
        // CMD9 failed
        sd_spi_deselect();

        return m_Status;
    }
    // Set block length to 512 (CMD16)
    if (sd_cmd(CMD16_SET_BLOCKLEN, _block_size, false, 0) != 0) {
        DBG_PRINTF("Set %" PRIu32 "-byte block timed out\r\n", _block_size);
        sd_spi_deselect();

        return m_Status;
    }
    // Set SCK for data transfer
    sd_spi_go_high_frequency();

    // The card is now initialized
    m_Status &= ~STA_NOINIT;

    sd_spi_deselect();

    // Return the disk status
    return m_Status;
}

static uint tx_dma;
static uint rx_dma;
static dma_channel_config tx_dma_cfg;
static dma_channel_config rx_dma_cfg;

bool sd_spi_write_blocking(const uint8_t* tx, size_t length)
{
    static uint8_t dummy = 0xA5;
    dma_channel_configure(tx_dma, &tx_dma_cfg,
                          &spi_get_hw(SD_SPI_PORT)->dr,  // write address
                          tx,                              // read address
                          length,  // element count (each element is of
                                   // size transfer_data_size)
                          false);  // start
    dma_channel_configure(rx_dma, &rx_dma_cfg,
                        &dummy,  // write address
                        &spi_get_hw(SD_SPI_PORT)->dr,// read address
                        length,  // element count (each element is of
                                // size transfer_data_size)
                        false);  // start
    dma_start_channel_mask((1u << tx_dma) | (1u << rx_dma));

    dma_channel_wait_for_finish_blocking(tx_dma);
    dma_channel_wait_for_finish_blocking(rx_dma);

    assert(!dma_channel_is_busy(tx_dma));
    assert(!dma_channel_is_busy(rx_dma));

    return true;
}


bool sd_init_driver()
{
    static bool initialized = false;
    if (!initialized) {
        // Chip select is active-low, so we'll initialise it to a
        // driven-high state.
        gpio_put(SD_CS_PIN, 1);
        // Avoid any glitches when enabling output
        gpio_init(SD_CS_PIN);
        gpio_set_dir(SD_CS_PIN, GPIO_OUT);
        gpio_put(SD_CS_PIN, 1); // In case set_dir does anything

        spi_init(SD_SPI_PORT, 100000);
        spi_set_format(SD_SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
        gpio_set_function(SD_MISO_PIN, GPIO_FUNC_SPI);
        gpio_set_function(SD_MOSI_PIN, GPIO_FUNC_SPI);
        gpio_set_function(SD_SCK_PIN, GPIO_FUNC_SPI);

        gpio_set_drive_strength(SD_MISO_PIN, GPIO_DRIVE_STRENGTH_4MA);
        gpio_set_drive_strength(SD_SCK_PIN, GPIO_DRIVE_STRENGTH_2MA);

        gpio_pull_up(SD_MISO_PIN);

        tx_dma = dma_claim_unused_channel(true);
        tx_dma_cfg = dma_channel_get_default_config(tx_dma);
        channel_config_set_transfer_data_size(&tx_dma_cfg, DMA_SIZE_8);
        channel_config_set_dreq(&tx_dma_cfg, DREQ_SPI0_TX);
        channel_config_set_write_increment(&tx_dma_cfg, false);
        channel_config_set_read_increment(&tx_dma_cfg, true);

        rx_dma = dma_claim_unused_channel(true);
        rx_dma_cfg = dma_channel_get_default_config(rx_dma);
        channel_config_set_transfer_data_size(&rx_dma_cfg, DMA_SIZE_8);
        channel_config_set_dreq(&rx_dma_cfg, DREQ_SPI0_RX);
        channel_config_set_write_increment(&rx_dma_cfg, false);
        channel_config_set_read_increment(&rx_dma_cfg, false);

        initialized = true;
    }
    return true;
}

/* [] END OF FILE */

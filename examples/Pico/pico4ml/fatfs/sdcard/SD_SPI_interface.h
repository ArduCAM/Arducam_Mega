#ifndef _SD_SPI_INTERFACE_H_
#define _SD_SPI_INTERFACE_H_
#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SD_SPI_PORT   spi0
#define SD_CS_PIN     7
#define SPI_FILL_CHAR (0xFF)

// inline unsigned char sd_spi_transport_byte(unsigned char wdata) {
//     unsigned char rdata;
//     spi_write_read_blocking(SD_SPI_PORT, &wdata, &rdata, 1);
//     return rdata;
// }

// inline unsigned char sd_spi_write_block(unsigned char *wdata, size_t len) {
//     return spi_write_blocking(SD_SPI_PORT, wdata, len);
// }

// inline unsigned char sd_spi_read_block(unsigned char *rdata, size_t len) {
//     return spi_read_blocking(SD_SPI_PORT, 0xff, rdata, len);
// }

// Would do nothing if SD_CS_PIN were set to GPIO_FUNC_SPI.
void sd_spi_select() {
    gpio_put(SD_CS_PIN, 0);
    // A fill byte seems to be necessary, sometimes:
    uint8_t fill = SPI_FILL_CHAR;
    spi_write_blocking(SD_SPI_PORT, &fill, 1);
}

void sd_spi_deselect() {
    gpio_put(SD_CS_PIN, 1);

    /*
    MMC/SDC enables/disables the DO output in synchronising to the SCLK. This
    means there is a posibility of bus conflict with MMC/SDC and another SPI
    slave that shares an SPI bus. Therefore to make MMC/SDC release the MISO
    line, the master device needs to send a byte after the CS signal is
    deasserted.
    */
    uint8_t fill = SPI_FILL_CHAR;
    spi_write_blocking(SD_SPI_PORT, &fill, 1);
}

bool sd_spi_transfer(const uint8_t *tx, uint8_t *rx, size_t length) {
    return spi_write_read_blocking(SD_SPI_PORT, tx, rx, length);
}

bool sd_spi_read_blocking(uint8_t *rx, size_t length) {
    return spi_read_blocking(SD_SPI_PORT, 0xff, rx, length);
}

uint8_t sd_spi_write(const uint8_t value) {
    uint8_t received = SPI_FILL_CHAR;
    int num = spi_write_read_blocking(SD_SPI_PORT, &value, &received, 1);
    return received;
}

/* Some SD cards want to be deselected between every bus transaction */
void sd_spi_deselect_pulse() {
    sd_spi_deselect();
    // tCSH Pulse duration, CS high 200 ns
    sleep_us(200);
    sd_spi_select();
}

void sd_spi_go_high_frequency() {
    uint actual = spi_set_baudrate(SD_SPI_PORT, SPI0_BAUDRATE);
    printf("%s: Actual frequency: %lu\n", __FUNCTION__, (long)actual);
}
void sd_spi_go_low_frequency() {
    uint actual =
        spi_set_baudrate(SD_SPI_PORT, 400 * 1000); // Actual frequency: 398089
    printf("%s: Actual frequency: %lu\n", __FUNCTION__, (long)actual);
}

void sd_spi_send_initializing_sequence() {
    bool old_ss = gpio_get(SD_CS_PIN);
    // Set DI and CS high and apply 74 or more clock pulses to SCLK:
    gpio_put(SD_CS_PIN, 1);
    uint8_t ones[10];
    memset(ones, 0xFF, sizeof ones);
    absolute_time_t timeout_time = make_timeout_time_ms(1);
    do {
        sd_spi_transfer(ones, NULL, sizeof ones);
    } while (0 < absolute_time_diff_us(get_absolute_time(), timeout_time));
    gpio_put(SD_CS_PIN, old_ss);
}

void sd_spi_init_pl022() {
    // Let the PL022 SPI handle it.
    // the CS line is brought high between each byte during transmission.
    gpio_set_function(SD_CS_PIN, GPIO_FUNC_SPI);
}

#ifdef __cplusplus
}
#endif
#endif
#include "ff.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include "hardware/spi.h"
#include <string.h>
FATFS microSDFatFs;

void SD_Init(void)
{
    // Check the mounted device
    FRESULT f_res = f_mount(&microSDFatFs, (TCHAR const*)"/", 1);
    if (f_res != FR_OK) {
        printf("SD card mount file system failed ,error code :(%d)\r\n", f_res);
    } else {
        printf("SD card mount file system success!! \r\n");
    }
}

#define SPI_PORT    spi0
#define MISO_PIN    16
#define MOSI_PIN    19
#define SCK_PIN     18

/*Init spi bus and cs */
void fatfs_spi_init(uint32_t spi_baudrate)
{
    spi_init(SPI_PORT, spi_baudrate);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
}


int main()
{
    char filename[20];
    stdio_init_all();
    sleep_ms(3000);
    FIL fp;
    UINT btw;
    char buff[2048];
    fatfs_spi_init(12000000);
    printf("start test fatfs\r\n");
    SD_Init();

    // sprintf(filename, "test.txt");
    // if ((f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE)) == FR_OK) {
    // f_
    // }
    for (size_t i = 0; i < 100; i++)
    {
        char temp = i+'0'%('9'+1);
        memset(buff,temp,2048);
        sprintf(filename, "test_%03d.txt", i);
        if ((f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
            printf("f_open fail in %d\r\n",i);
        }
        else {
            sleep_ms(100);
            f_write(&fp,buff,2048,&btw);
            printf("[%d]write %d\r\n",i,btw);
        }
        sleep_ms(100);
        f_close(&fp);
    }
    printf("complete!");
    return 0;
}
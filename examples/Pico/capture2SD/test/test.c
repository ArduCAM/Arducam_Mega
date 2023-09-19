#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "hardware/spi.h"
#include <string.h>
#include "ff.h"
#include "ArducamCamera.h"
FATFS microSDFatFs;
ArducamCamera CAM;

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
#define READ_IMAGE_LENGTH 1024
uint8_t frame_buffer[READ_IMAGE_LENGTH] = {0};

int main()
{
    char filename[40];
    stdio_init_all();
    sleep_ms(3000);
    UINT btw;
    printf("start test fatfs\r\n");
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    gpio_put(20, 1);

    gpio_init(28);
    gpio_set_dir(28, GPIO_OUT);
    gpio_put(28, 0);
    SD_Init();
    CAM = createArducamCamera(13);
    begin(&CAM);
    printf("start Mega\r\n");
    FIL fp;
    for (size_t i = 0; i < 10; i++)
    {
        memset(&fp,0,sizeof(FIL));
        memset(frame_buffer,(i%10)+'0',READ_IMAGE_LENGTH);
        sprintf(filename, "test_%03d.jpeg", rand());
        absolute_time_t now = get_absolute_time();
        int64_t sumtime = 0;
        if ((f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ)) != FR_OK) {
            printf("f_open fail in %d\r\n",i);
            break;
        }
        else {
            // sleep_ms(100);
            // f_write(&fp,frame_buffer,READ_IMAGE_LENGTH,&btw);
            // printf("[%d]write %d\r\n",i,btw);
            // gpio_put(28, 1);

            takePicture(&CAM, CAM_IMAGE_MODE_WQXGA2, CAM_IMAGE_PIX_FMT_JPG);
            while (CAM.receivedLength) {
                uint32_t rlen = readBuff(&CAM, frame_buffer, READ_IMAGE_LENGTH);
                absolute_time_t write_now = get_absolute_time();
                f_write(&fp,frame_buffer,rlen,&btw);
                int64_t tmp = absolute_time_diff_us(write_now,get_absolute_time());
                // printf("[%d]read %u save %u,Spend time:%lldus \r\n",i,rlen,btw,tmp);
                sumtime += tmp;
            }

        }
        f_close(&fp);
        // gpio_put(28, 0);

        printf("[%d]save %s length:%uB Spend time:%lldus\r\n",i,filename,CAM.totalLength,absolute_time_diff_us(now,get_absolute_time()));
        printf("Writing to sd card takes time: %lldus\r\n",sumtime);
        sleep_ms(2000);
    }
    f_mount(0,(TCHAR const*)"/",0);
    printf("complete!");
    return 0;
}
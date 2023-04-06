#include "main.h"
#include "hardware/watchdog.h"
#include "time.h"
FATFS microSDFatFs;
UINT btw;
FIL fp;

static enum save_picture_state _save_state = save_picture_idle;
CAM_IMAGE_MODE picture_resolution          = CAM_IMAGE_MODE_FHD;
char* array_label[]                        = {"WIFI", "<---", "1600X1200", "1920X1080", "2048X1536"};
CAM_IMAGE_MODE camera_resoultion[]         = {CAM_IMAGE_MODE_UXGA, CAM_IMAGE_MODE_FHD, CAM_IMAGE_MODE_QXGA};

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

struct ClassCamera CAM = {0, 0};
volatile bool reflash  = false;
void core1_entry()
{
    CAM.cam    = createArducamCamera(13);
    CAM.status = Camera_init;

    while (true) {
        if (cameraHeartBeat(&CAM.cam)) {
            switch (CAM.status) {
            case Camera_close:
                sleep_ms(50);
            case Camera_init:
                begin(&CAM.cam);
                CAM.status = Camera_open;
                if (CAM.cam.cameraId == SENSOR_5MP_1) {
                    array_label[4]       = "2592X1944";
                    camera_resoultion[2] = CAM_IMAGE_MODE_WQXGA2;
                } else if (CAM.cam.cameraId == SENSOR_5MP_2) {
                    array_label[4]       = "2592X1936";
                    camera_resoultion[2] = CAM_IMAGE_MODE_WQXGA2;
                } else {
                    array_label[4]       = "2048X1536";
                    camera_resoultion[2] = CAM_IMAGE_MODE_QXGA;
                }
                reflash = true;
                break;
            case Camera_open:
                if (_save_state == save_picture_idle)
                    camera_get_picture(&CAM.cam);
                else if (_save_state == save_picture_runnig) {
                    camera_save_picture(&CAM.cam, picture_resolution);
                    _save_state = save_picture_complete;
                } else if (_save_state == wifi_server_run)
                    server_get_picture(&CAM.cam);
                break;
            }
        } else {
            CAM.status = Camera_close;
        }
    }
}

void diplay_label(uint8_t i, uint16_t color)
{
    if (i < 2)
        display_string(label_loc[i][0], label_loc[i][1], array_label[i], BLACK, 24);
    else
        display_string(label_loc[i][0], label_loc[i][1], array_label[i], color, 16);
}

void draw_ui()
{
    display_color(win_loc, BLACK);
    display_image(camera_loc, camera_loc.logo_data);
    for (uint8_t i = 0; i < 5; i++) {
        draw_rect(array_btn[i], BLACK);
        diplay_label(i, GRAY);
    }
}

int main()
{
    char filename[20];
    int click_btn_idx        = -1;
    uint8_t wifiserver_state = false, find_jpg_head = false;
    pico4ml_init(false);
    pico4ml_camera_spi_init(8 * 1000 * 1000);
    uint32_t wait_cnt = 0;
    irq_set_exclusive_handler(UART_IRQ, uart_rx_handler);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
    printf("full_featured\n");
    multicore_launch_core1(core1_entry);
    clear_screen(WHITE);
    draw_ui();
    SD_Init();
    int image_save_cnt = 0;
    while (true) {
        if (reflash == true) {
            reflash = false;
            diplay_label(4, GRAY);
        }
        switch (_save_state) {
        case save_picture_start:
            sprintf(filename, "img%04d.jpg", image_save_cnt++);
            draw_rect(win_loc, GRAY);
            /* Create the file */
            if ((f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
                _save_state = save_picture_error;
            } else {
                save_buffer_ready = false;
                find_jpg_head     = true;
                _save_state       = save_picture_runnig;
            }
            break;
        case save_picture_runnig:
            if (save_buffer_ready == true) {
                int write_len = 0;

                if ((read_buffer_lenght - write_len) > 0) {
                    do {
                        f_write(&fp, frame_buff + write_len, read_buffer_lenght - write_len, &btw);
                        write_len += btw;
                    } while (write_len != read_buffer_lenght);
                }
                save_buffer_ready = false;
                sleep_us(200);
            }
            break;
        case save_picture_complete:
            f_close(&fp);
            sleep_ms(10);
            _save_state          = save_picture_idle;
            preview_buffer_ready = false;
            diplay_label(click_btn_idx, GRAY);
            draw_rect(win_loc, GREEN);
            break;
        case save_picture_idle:
            click_btn_idx = select_touch(array_btn, 5);
            if (++wait_cnt > 300000)
                wait_cnt = 3000001;
            if (wait_cnt > 300000 && click_btn_idx != -1) {
                switch (click_btn_idx) {
                case 0:
                    _save_state = wifi_server_run;
                    wifi_server_start();
                    draw_rect(array_btn[0], GREEN);
                    wait_cnt = 0;
                    break;
                case 1:
                    f_unmount((TCHAR const*)"/");
                    watchdog_enable(10, 1);
                    watchdog_enable_caused_reboot();
                    watchdog_caused_reboot();
                    break;
                default:
                    _save_state = save_picture_start;
                    // while (save_buffer_ready != true)
                    //     tight_loop_contents();
                    save_buffer_ready  = false;
                    picture_resolution = camera_resoultion[click_btn_idx - 2];
                    diplay_label(click_btn_idx, GREEN);
                    break;
                }
            }

            if (preview_buffer_ready == true) {
                display_image(preview_loc, frame_buff);
                preview_buffer_ready = false;
            }
            break;
        case save_picture_error:
            _save_state          = save_picture_idle;
            preview_buffer_ready = false;
            diplay_label(click_btn_idx, GRAY);
            draw_rect(win_loc, RED);
            break;
        case wifi_server_run:
            wifi_server_process();
            if (wifiserver_state == false) {
                wifiserver_state = get_wifi_server_stats();
                if (wifiserver_state == true) {
                    draw_rect(array_btn[0], GREEN);
                } else {
                    draw_rect(array_btn[0], RED);
                }
            }
            if (wait_cnt > 300000) {
                switch (select_touch(&array_btn[0], 2)) {
                case 0:
                    preview_buffer_ready = false;
                    _save_state          = save_picture_idle;
                    wifi_buffer_ready    = false;
                    draw_rect(array_btn[0], BLACK);
                    wait_cnt = 0;
                    break;
                case 1:
                    f_unmount((TCHAR const*)"/");
                    watchdog_enable(10, 1);
                    watchdog_enable_caused_reboot();
                    watchdog_caused_reboot();
                    break;
                }
            } else
                wait_cnt++;
            break;
        }
    }

    return 0;
}

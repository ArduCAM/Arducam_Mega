#include "main.h"
#include <string.h>
char* htmls[] = {
    "HTTP/1.0 404 NOT FOUND \r\nServer: jdbhttpd/0.1.0\r\nContent-Type: "
    "text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n\r\n<BODY><P>The "
    "server could not fulfill\r\n your request because the \r\nresource "
    "specified\r\nis unavailable or nonexistent.\r\n</BODY></HTML>",
    "HTTP/1.1 200 OK\r\nServer: jdbhttpd/0.1.0\r\nContent-Type: "
    "image/jpeg\r\n\r\n",
};
char buff_string[80];

void send_at_cmd(const char cmd[])
{
    printf("AT+%s\r\n", cmd);
}

enum { AT_READY = 0, AT_INIT, AT_RUN, AT_ERR } esp_state = AT_INIT;

volatile int esp_cmd_idk = 1;
volatile bool send_flag = true;
char uart_rx_offset = 0;

char UART_READ[] = "ready";

void AT_ready(uint8_t data)
{
    if (data == UART_READ[uart_rx_offset]) {
        uart_rx_offset++;
        if (uart_rx_offset >= 5) {
            uart_rx_offset = 0;
            esp_state = AT_INIT;
            esp_cmd_idk = 1;
            send_flag = true;
        }
    } else {
        uart_rx_offset = 0;
    }
}

char UART_RETURN_OK[] = "OK";
char UART_RETURN_ERROR[] = "ERR";
void AT_init(uint8_t data)
{
    if (data == UART_RETURN_OK[uart_rx_offset]) {
        uart_rx_offset++;
        if (uart_rx_offset >= 2) {
            uart_rx_offset = 0;
            if (++esp_cmd_idk > 5)
                esp_state = AT_RUN;
            else
                send_flag = true;
        }
    } else if (data == UART_RETURN_ERROR[uart_rx_offset]) {
        uart_rx_offset++;
        if (uart_rx_offset >= 3) {
            uart_rx_offset = 0;
            send_flag = true;
        }
    } else {
        uart_rx_offset = 0;
    }
}

enum {
    run_idle = 0,
    run_get_client_id,
    run_get_api_head,
    run_get_api_interface,
    run_error,
} run_state = run_idle;

const char uart_rev_ipd[] = "+IPD,";
char uart_rev_get[] = "GET /";
char client_id[10];
uint8_t client_id_cnt = 0;
uint8_t send_index = 0;
void AT_run(uint8_t data)
{
    switch (run_state) {
    case run_idle:
        if (data == uart_rev_ipd[uart_rx_offset]) {
            uart_rx_offset++;
            if (uart_rx_offset >= 5) {
                uart_rx_offset = 0;
                esp_cmd_idk = 6;
                run_state = run_get_client_id;
                client_id_cnt = 0;
            }
        } else if (data == '>') {
            send_flag = true;
            esp_cmd_idk = 7;
        } else {
            uart_rx_offset = 0;
        }
        break;
    case run_get_client_id:
        client_id[client_id_cnt] = data;
        if (client_id[client_id_cnt++] == ',') {
            client_id[client_id_cnt - 1] = 0;
            run_state = run_get_api_head;
        }
        break;
    case run_get_api_head:
        if (data == uart_rev_get[uart_rx_offset]) {
            uart_rx_offset++;
            if (uart_rx_offset >= 5) {
                uart_rx_offset = 0;
                run_state = run_get_api_interface;
                client_id_cnt = 0;
            }
        } else {
            uart_rx_offset = 0;
        }
        break;
    case run_get_api_interface:
        if (data == 'g') {
            send_index = 1;
            send_flag = true;
            run_state = run_idle;
        } else {
            send_index = 0;
            send_flag = true;
            run_state = run_idle;
        }
        break;
    }
}

void uart_rx_handler()
{
    uint8_t data = uart_getc(UART_ID);
    switch (esp_state) {
    case AT_READY:
        AT_ready(data);
        break;
    case AT_INIT:
        AT_init(data);
        break;
    case AT_RUN:
        AT_run(data);
        break;
    default:
        break;
    }
}
volatile bool wifi_buffer_ready = false;
char* at_cmd[] = {"RESTORE",                                          // 0
                  "CWMODE=2",                                         // 1
                  "CIPMUX=1",                                         // 3
                  "CWSAP=\"arducam_pico4ml\",\"1234567890\",5,0,1,0", // 2
                  "CIPSERVER=1,8080",                                 // 4
                  "CIPSTA?",                                          // 5
                  "CIPSEND",                                          // 6
                  "CIPCLOSE"};                                        // 7

void server_get_picture(ArducamCamera* camera)
{
    int rs = 0;
    while (wifi_buffer_ready != false)
        tight_loop_contents();
    takePicture(camera, CAM_IMAGE_MODE_QVGA, CAM_IMAGE_PIX_FMT_JPG);
    do {
        rs = readBuff(camera, frame_buff, camera->totalLength);
    } while (rs <= 0);
    wifi_buffer_ready = true;
}
void wifi_server_start()
{
    if (esp_cmd_idk < 6)
        send_flag = true;
}

uint8_t get_wifi_server_stats()
{
    return esp_cmd_idk >= 6;
}

void wifi_server_process()
{
    if (send_flag || esp_cmd_idk == 0) {
        send_flag = false;
        if (esp_cmd_idk < 6) {
            send_at_cmd(at_cmd[esp_cmd_idk]);
            if (esp_cmd_idk == 0)
                sleep_ms(1000);
        } else if (esp_cmd_idk == 6) {
            int len = 0;
            if (send_index == 0) {
                len = strlen(htmls[send_index]);
            } else {
                if (CAM.status == Camera_open) {
                    wifi_buffer_ready = false;
                    while (wifi_buffer_ready != true)
                        tight_loop_contents();
                    len = CAM.cam.totalLength + strlen(htmls[send_index]);
                } else {
                    len = 0;
                    esp_cmd_idk = 7;
                    send_index = -1;
                }
            }
            if (len >= 8192) {
                sprintf(buff_string, "%sL=%s,%d", at_cmd[esp_cmd_idk], client_id, len);
            } else {
                sprintf(buff_string, "%s=%s,%d", at_cmd[esp_cmd_idk], client_id, len);
            }
            send_at_cmd(buff_string);

        } else if (esp_cmd_idk == 7) {
            switch (send_index) {
            case 0:
                printf(htmls[send_index]);
                break;
            case 1:
                printf(htmls[send_index]);
                if (wifi_buffer_ready == true) {
                    // int rs = readBuff(&CAM.cam, frame_buff, CAM.cam.totalLength);
                    uart_write_blocking(uart0, frame_buff, CAM.cam.totalLength);
                    wifi_buffer_ready = false;
                }
                break;
            }
            sleep_ms(1);
            sprintf(buff_string, "%s=%s", at_cmd[esp_cmd_idk], client_id);
            send_at_cmd(buff_string);
        }
    }
}
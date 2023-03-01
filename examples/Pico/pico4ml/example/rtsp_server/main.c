
#include <stdlib.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "http_server.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

char ssid[] = "Arducam";
char pass[] = "20160224";
uint32_t LocalTime = 0; /* this variable is used to create a time reference
                           incremented by 10ms */

int main() {
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK)) {
        printf("failed to initialise\n");
        return 1;
    }
    printf("initialised\n");

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK,
                                           10000)) {
        printf("failed to connect\n");
        return 1;
    }
    printf("connected\n");
    httpd_init();
    // run_tcp_server_test();
    while (/* condition */ 1) {
        tight_loop_contents();
    }

    cyw43_arch_deinit();
}
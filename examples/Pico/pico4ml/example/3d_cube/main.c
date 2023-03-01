#include "3d_cube.h"
#include "ICM42622.h"
#include "apps_config.h"
#include "gui_func.h"

int main() {
    pico4ml_init(true);
    
    clear_screen( WHITE);
    icm42622_init();

    setup();
    while (1) {
        loop();
    }

    return 0;
}
#include "multitude_camera_app.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "apps_config.h"
int main()
{
    pico4ml_init(true);
    pico4ml_camera_spi_init(8 * 1000 * 1000);
    multicore_launch_core1(display_loop);
    capture_loop();
    
}

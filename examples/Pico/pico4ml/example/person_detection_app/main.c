#include "person_detection_app.h"
#include "apps_config.h"

int main()
{
    pico4ml_init(true);    
    printf("person_detection_app\n");
    app_loop();
}


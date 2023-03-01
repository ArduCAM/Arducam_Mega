cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(3D_CUBE_APP)
set(EXAMPLE_3D_CUBE_APP 3d_cube_app)

include_directories(src)
include_directories(../src)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(src 3D_CUBE_APP_SOURCE)

# message(${RTSP_SERVER_SOURCE})
add_executable(${EXAMPLE_3D_CUBE_APP} main.c ${APP_CONFIG_SRC} ${3D_CUBE_APP_SOURCE})

pico_enable_stdio_usb(${EXAMPLE_3D_CUBE_APP} 0)
pico_enable_stdio_uart(${EXAMPLE_3D_CUBE_APP} 1)
target_link_libraries(${EXAMPLE_3D_CUBE_APP} hardware_i2c hardware_uart pico_multicore liblcd ICM42622)
pico_add_extra_outputs(${EXAMPLE_3D_CUBE_APP})
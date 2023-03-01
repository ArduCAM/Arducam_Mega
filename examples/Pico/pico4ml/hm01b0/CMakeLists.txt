cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(HM01B0_LIB C CXX ASM)
 
add_library(arducam_hm01b0 INTERFACE)
pico_generate_pio_header(arducam_hm01b0 ${CMAKE_CURRENT_LIST_DIR}/image.pio)

target_sources(arducam_hm01b0 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/arducam_hm01b0.c)
target_link_libraries(arducam_hm01b0 INTERFACE hardware_pio pico_stdlib hardware_i2c hardware_dma)

# project(HM01B0_APP)
# set(EXAMPLE_HM01B0 hm01b0_app)
# # include_directories(src)
# include_directories(../src)
# aux_source_directory(../src APP_CONFIG_SRC)
# # aux_source_directory(src 3D_CUBE_APP_SOURCE)
# # message(${RTSP_SERVER_SOURCE})
# add_executable(${EXAMPLE_HM01B0} main.c ${APP_CONFIG_SRC})
# pico_enable_stdio_usb(${EXAMPLE_HM01B0} 0)
# pico_enable_stdio_uart(${EXAMPLE_HM01B0} 1)
# target_link_libraries(${EXAMPLE_HM01B0} hardware_i2c hardware_uart pico_multicore liblcd arducam_hm01b0)
# pico_add_extra_outputs(${EXAMPLE_HM01B0})
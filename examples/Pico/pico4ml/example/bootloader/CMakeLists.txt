cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(BOOTLOADER)

set(EXAMPLE_MAIN bootloader)

include_directories(src)
include_directories(../src)

aux_source_directory(src EXAMPLE_MAIN_SRC)
aux_source_directory(../src APP_CONFIG_SRC)

add_executable(${EXAMPLE_MAIN}  main.c ${APP_CONFIG_SRC} ${EXAMPLE_MAIN_SRC})
# enable usb output, disable uart output
pico_enable_stdio_usb(${EXAMPLE_MAIN} 0)
pico_enable_stdio_uart(${EXAMPLE_MAIN} 1)
# create map/bin/hex/uf2 file etc.
target_link_libraries(${EXAMPLE_MAIN}  hardware_i2c  hardware_uart liblcd hardware_flash hardware_dma cmsis_core fatfs)
# set_linker_script(${EXAMPLE_MAIN} memmap_application.ld) #set app offset address
pico_add_extra_outputs(${EXAMPLE_MAIN})

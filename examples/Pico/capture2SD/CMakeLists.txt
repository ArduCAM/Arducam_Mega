cmake_minimum_required(VERSION 3.12)  #set the cmake need version

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Sdcard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

# aux_source_directory(. DIR_SDCARD_SRCS)
aux_source_directory(fatfs DIR_FATFS_SRCS)
# aux_source_directory(time DIR_TIME_SRCS)
set( DIR_CAM_SRCS
    ../../../../src/Arducam/picoHal.h
    Arducam/cam_spi_master.h
    ../../../../src/Arducam/ArducamCamera.h
    Arducam/cam_spi_master.c 
    ../../../../src/Arducam/ArducamCamera.c
    )

include_directories(fatfs)
# include_directories(time)
include_directories(../../../src/Arducam)
include_directories(Arducam)

# #${DIR_TIME_SRCS}
# add_library(fatfs ${DIR_FATFS_SRCS} ${DIR_SDCARD_SRCS})
# # hardware_rtc
# target_link_libraries(fatfs PUBLIC hardware_spi pico_stdlib)


add_executable(testFatfs test/test.c ${DIR_FATFS_SRCS} ${DIR_CAM_SRCS})
pico_enable_stdio_usb(testFatfs 1)
pico_enable_stdio_uart(testFatfs 0)
target_link_libraries(testFatfs PRIVATE hardware_spi hardware_dma pico_stdlib)
# target_link_libraries(testFatfs fatfs)
pico_add_extra_outputs(testFatfs)
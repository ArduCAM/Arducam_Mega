project(Sdcard C CXX ASM)
aux_source_directory(. DIR_FATFS_SRCS)
aux_source_directory(sdcard DIR_SDCARD_SRCS)
# aux_source_directory(time DIR_TIME_SRCS)

include_directories(sdcard)
# include_directories(time)

#${DIR_TIME_SRCS}
add_library(fatfs ${DIR_FATFS_SRCS} ${DIR_SDCARD_SRCS})
# hardware_rtc
target_link_libraries(fatfs PUBLIC hardware_spi pico_stdlib)


add_executable(testFatfs test/test.c)
pico_enable_stdio_usb(testFatfs 1)
pico_enable_stdio_uart(testFatfs 0)

target_link_libraries(testFatfs fatfs)
pico_add_extra_outputs(testFatfs)
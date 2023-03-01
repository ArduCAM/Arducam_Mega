project(Liblcd C CXX ASM)

aux_source_directory(. DIR_LCD_SRCS)
aux_source_directory(font DIR_FONT_SRCS)

include_directories(font)
include_directories(lcd_driver)

add_library(liblcd ${DIR_LCD_SRCS} ${DIR_FONT_SRCS})
target_link_libraries(liblcd PUBLIC pico_stdlib hardware_spi)

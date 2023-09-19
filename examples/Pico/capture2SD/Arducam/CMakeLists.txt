# aux_source_directory(. DIR_CAM_SRCS)
project(Arducam)

set( DIR_CAM_SRCS
    ../../../../src/Arducam/picoHal.h
    cam_spi_master.h
    ../../../../src/Arducam/ArducamCamera.h
    cam_spi_master.c 
    ../../../../src/Arducam/ArducamCamera.c
    )
add_library(libarducam ${DIR_CAM_SRCS})
target_link_libraries(libarducam PUBLIC hardware_spi pico_stdlib)

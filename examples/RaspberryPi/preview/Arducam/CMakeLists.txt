# aux_source_directory(. DIR_CAM_SRCS)
project(Arducam)
add_definitions(-D__PLATFORM_RASPBERRY)

set( DIR_CAM_SRCS
    ../../../../src/Arducam/picoHal.h
    bsp_spi.h
    ../../../../src/Arducam/ArducamCamera.h
    bsp_spi.c 
    ../../../../src/Arducam/ArducamCamera.c
    )
add_library(libarducam SHARED ${DIR_CAM_SRCS})
# target_link_libraries(libarducam)

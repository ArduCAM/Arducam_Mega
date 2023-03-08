project(Arducam)
set( DIR_CAM_SRCS
    ../../../../src/Arducam/picoHal.h
    cam_spi_master.h
    ../../../../src/Arducam/ArducamCamera.h
    ../../../../src/Arducam_Mega.h
    cam_spi_master.c 
    ../../../../src/Arducam/ArducamCamera.c
    ../../../../src/Arducam_Mega.cpp
    )
add_library(libarducam ${DIR_CAM_SRCS})
target_link_libraries(libarducam PUBLIC hardware_spi pico_stdlib )

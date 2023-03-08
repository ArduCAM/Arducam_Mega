# aux_source_directory(. DIR_CAM_SRCS)
project(Arducam)
set( DIR_CAM_SRCS
    ../../../../src/Arducam/picoHal.h
    cam_spi_master.h
    ../../../../src/Arducam/ArducamCamera.h
    # ../../../../src/Arducam_Mega.h
    cam_spi_master.c 
    ../../../../src/Arducam/ArducamCamera.c
    # ../../../../src/Arducam_Mega.cpp
    )
add_library(libarducam ${DIR_CAM_SRCS})
target_link_libraries(libarducam PUBLIC hardware_spi pico_stdlib )
project(TestArducam)
add_executable(testarducam test.c ${DIR_CAM_SRCS} )
target_link_libraries(testarducam PUBLIC hardware_spi pico_stdlib pico_multicore)
pico_enable_stdio_usb(testarducam 1)
pico_enable_stdio_uart(testarducam 0)
pico_add_extra_outputs(testarducam)

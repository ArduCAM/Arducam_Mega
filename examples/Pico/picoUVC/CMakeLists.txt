cmake_minimum_required(VERSION 3.5)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(PROJECT dev_video_capture)
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

include_directories(Arducam)
include_directories(../../../src)

add_subdirectory(Arducam)

add_executable(${PROJECT})

# Example source
target_sources(${PROJECT} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
)

# Example include
target_include_directories(${PROJECT} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT} PUBLIC pico_stdlib tinyusb_device tinyusb_board libarducam pico_multicore)

# Configure compilation flags and libraries for the example... see the corresponding function
# in hw/bsp/FAMILY/family.cmake for details.
pico_add_extra_outputs(${PROJECT})

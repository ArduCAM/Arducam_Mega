cmake_minimum_required(VERSION 3.12)

#include(pico-sdk/pico_sdk_init.cmake)
# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(MEGA_FULLFEATURED C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()
include_directories(../../../src)
include_directories(Arducam)

add_subdirectory(Arducam)
aux_source_directory(. FULLFEATURED_SRC)
add_executable(full_featured ${FULLFEATURED_SRC})

target_link_libraries(full_featured pico_stdlib hardware_spi hardware_uart libarducam)

pico_add_extra_outputs(full_featured)

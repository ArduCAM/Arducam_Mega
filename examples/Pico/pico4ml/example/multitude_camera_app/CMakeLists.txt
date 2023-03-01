cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(MULTITUDE_CAMERA_APP)

set(EXAMPLE_MULTITUDE_CAMERA multitude_camera_app)

include_directories(src)
include_directories(../src)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(src EXAMPLE_MULTITUDE_CAMERA_SRC)

add_executable(${EXAMPLE_MULTITUDE_CAMERA} main.c ${APP_CONFIG_SRC} ${EXAMPLE_MULTITUDE_CAMERA_SRC})
# enable usb output, disable uart output
pico_enable_stdio_usb(${EXAMPLE_MULTITUDE_CAMERA} 0)
pico_enable_stdio_uart(${EXAMPLE_MULTITUDE_CAMERA} 1)
# create map/bin/hex/uf2 file etc.
target_link_libraries(${EXAMPLE_MULTITUDE_CAMERA}  hardware_i2c  hardware_uart libarducam liblcd pico_multicore)

pico_add_extra_outputs(${EXAMPLE_MULTITUDE_CAMERA})

target_link_directories(${EXAMPLE_MULTITUDE_CAMERA} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_MULTITUDE_CAMERA} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)

# # Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_MULTITUDE_CAMERA} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_MULTITUDE_CAMERA} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)

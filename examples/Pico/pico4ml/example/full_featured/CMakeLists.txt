cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(FULL_FEATURED_APP)
set(EXAMPLE_FULL_FEATURED full_featured_app)

include_directories(../src)
include_directories(./src)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(./src FULL_FEATURED_SOURCE)

# message(${RTSP_SERVER_SOURCE})
add_executable(${EXAMPLE_FULL_FEATURED} main.c ${FULL_FEATURED_SOURCE} ${APP_CONFIG_SRC})

pico_enable_stdio_usb(${EXAMPLE_FULL_FEATURED} 0)
pico_enable_stdio_uart(${EXAMPLE_FULL_FEATURED} 1)
target_link_libraries(${EXAMPLE_FULL_FEATURED} libarducam pico_multicore liblcd fatfs)
pico_add_extra_outputs(${EXAMPLE_FULL_FEATURED})

target_link_directories(${EXAMPLE_FULL_FEATURED} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_FULL_FEATURED} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)

# # Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_FULL_FEATURED} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_FULL_FEATURED} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)
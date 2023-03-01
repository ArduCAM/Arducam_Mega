cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(MAGIC_WAND_APP)

set(EXAMPLE_MAGIC_WAND magic_wand_app)

include_directories(src)
include_directories(../src)

aux_source_directory(src EXAMPLE_MAGIC_WAND_SRC)
aux_source_directory(../src APP_CONFIG_SRC)

add_executable(${EXAMPLE_MAGIC_WAND} main.c ${APP_CONFIG_SRC} ${EXAMPLE_MAGIC_WAND_SRC})
# enable usb output, disable uart output
pico_enable_stdio_usb(${EXAMPLE_MAGIC_WAND} 0)
pico_enable_stdio_uart(${EXAMPLE_MAGIC_WAND} 1)
# create map/bin/hex/uf2 file etc.
target_link_libraries(${EXAMPLE_MAGIC_WAND}  hardware_i2c hardware_uart pico_multicore liblcd ICM42622 magic_wand)

# set_linker_script(${EXAMPLE_MAGIC_WAND} memmap_application.ld) #set app offset address
pico_add_extra_outputs(${EXAMPLE_MAGIC_WAND})
target_link_directories(${EXAMPLE_MAGIC_WAND} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_MAGIC_WAND} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)

# Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_MAGIC_WAND} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_MAGIC_WAND} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)

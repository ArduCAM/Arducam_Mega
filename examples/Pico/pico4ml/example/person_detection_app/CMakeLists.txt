cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(PERSON_DETECTION_APP)

set(EXAMPLE_PERSON_DETECTION person_detection_app)

include_directories(src)
include_directories(../src)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(src EXAMPLE_PERSON_DETECTION_SRC)

add_executable(${EXAMPLE_PERSON_DETECTION} main.c ${APP_CONFIG_SRC} ${EXAMPLE_PERSON_DETECTION_SRC} )
# enable usb output, disable uart output
pico_enable_stdio_usb(${EXAMPLE_PERSON_DETECTION} 0)
pico_enable_stdio_uart(${EXAMPLE_PERSON_DETECTION} 1)
# create map/bin/hex/uf2 file etc.
target_link_libraries(${EXAMPLE_PERSON_DETECTION}  hardware_i2c  hardware_uart arducam_hm01b0 liblcd  person_detection_int8)
# set_linker_script(${EXAMPLE_PERSON_DETECTION} memmap_application.ld) #set app offset address
pico_add_extra_outputs(${EXAMPLE_PERSON_DETECTION})

target_link_directories(${EXAMPLE_PERSON_DETECTION} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_PERSON_DETECTION} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)

# Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_PERSON_DETECTION} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_PERSON_DETECTION} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)
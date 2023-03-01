cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(MICRO_SPEECH_APP)

set(EXAMPLE_MICRO_SPEECH micro_speech_app)

include_directories(src)
include_directories(../src)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(src EXAMPLE_MICRO_SPEECH_SRC)

add_executable(${EXAMPLE_MICRO_SPEECH} main.c ${APP_CONFIG_SRC} ${EXAMPLE_MICRO_SPEECH_SRC})
# enable usb output, disable uart output
pico_enable_stdio_usb(${EXAMPLE_MICRO_SPEECH} 0)
pico_enable_stdio_uart(${EXAMPLE_MICRO_SPEECH} 1)
# create map/bin/hex/uf2 file etc.
target_link_libraries(${EXAMPLE_MICRO_SPEECH}  hardware_i2c  hardware_uart pico_multicore liblcd pico_pdm_microphone micro_speech)

# set_linker_script(${EXAMPLE_MICRO_SPEECH} memmap_application.ld) #set app offset address
pico_add_extra_outputs(${EXAMPLE_MICRO_SPEECH})

target_link_directories(${EXAMPLE_MICRO_SPEECH} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_MICRO_SPEECH} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)
# Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_MICRO_SPEECH} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_MICRO_SPEECH} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)
cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(USB_DEVICE_APP)
set(EXAMPLE_USB_DEVICE usb_device_app)
# set(FAMILY rp2040)
# set(BOARD pico_sdk)
# set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
include_directories(../src)
include_directories(.)

aux_source_directory(../src APP_CONFIG_SRC)
aux_source_directory(. USB_DEVICE_SOURCE)

# message(${RTSP_SERVER_SOURCE})
add_executable(${EXAMPLE_USB_DEVICE} ${USB_DEVICE_SOURCE} ${APP_CONFIG_SRC} )

# pico_enable_stdio_usb(${EXAMPLE_USB_DEVICE} 1)
# pico_enable_stdio_uart(${EXAMPLE_USB_DEVICE} 1)
target_link_libraries(${EXAMPLE_USB_DEVICE} pico_multicore fatfs tinyusb_device liblcd tinyusb_board)
pico_add_extra_outputs(${EXAMPLE_USB_DEVICE})
target_link_directories(${EXAMPLE_USB_DEVICE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
pico_set_linker_script(${EXAMPLE_USB_DEVICE} ${CMAKE_CURRENT_SOURCE_DIR}/memmap_application.ld)

# # Add dependencies on the 'included' linker scripts so that the target gets
# rebuilt if they are changed
pico_add_link_depend(${EXAMPLE_USB_DEVICE} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_defines.ld)
pico_add_link_depend(${EXAMPLE_USB_DEVICE} ${CMAKE_CURRENT_SOURCE_DIR}/../memmap_default.ld)
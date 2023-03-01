cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(RTSP_SERVER_APP C CXX ASM)
set(EXAMPLE_RTSP_SERVER rtsp_sever_app)


aux_source_directory(src RTSP_SERVER_SOURCE)

# message(${RTSP_SERVER_SOURCE})
add_executable(${EXAMPLE_RTSP_SERVER} main.c ${RTSP_SERVER_SOURCE})

pico_enable_stdio_usb(${EXAMPLE_RTSP_SERVER} 0)
pico_enable_stdio_uart(${EXAMPLE_RTSP_SERVER} 1)
pico_add_extra_outputs(${EXAMPLE_RTSP_SERVER})
target_include_directories(${EXAMPLE_RTSP_SERVER} PRIVATE ${CMAKE_CURRENT_LIST_DIR} src)
target_link_libraries(${EXAMPLE_RTSP_SERVER} pico_multicore pico_cyw43_arch_lwip_threadsafe_background pico_stdlib)

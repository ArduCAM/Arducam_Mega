cmake_minimum_required(VERSION 3.12)  #set the cmake need version

project(WEBSERVER_APP C CXX ASM)
set(EXAMPLE_WEBSERVER webserver_app)

include_directories(../src)
aux_source_directory(../src APP_CONFIG_SRC)
add_executable(${EXAMPLE_WEBSERVER} main.c ${APP_CONFIG_SRC})
target_link_libraries(${EXAMPLE_WEBSERVER} pico_multicore liblcd libarducam)
pico_add_extra_outputs(${EXAMPLE_WEBSERVER})
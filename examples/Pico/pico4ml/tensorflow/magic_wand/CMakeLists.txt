
cmake_minimum_required(VERSION 3.12)

project(magic_wand C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_library(magic_wand "")

target_include_directories(magic_wand
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/.
)

set_target_properties(
  magic_wand
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

target_sources(magic_wand
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/imu_provider.h
  ${CMAKE_CURRENT_LIST_DIR}/src/magic_wand_model_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/magic_wand_main_functions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/rasterize_stroke.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/magic_wand_model_data.h
  ${CMAKE_CURRENT_LIST_DIR}/magic_wand_main_functions.h
  ${CMAKE_CURRENT_LIST_DIR}/src/rasterize_stroke.h
)

target_link_libraries(
  magic_wand
  pico-tflmicro
  hardware_pwm
)


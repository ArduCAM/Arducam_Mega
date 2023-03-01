
cmake_minimum_required(VERSION 3.12)

project(person_detection C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_library(person_detection_int8 "")

target_include_directories(person_detection_int8
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/.
)

set_target_properties(
  person_detection_int8
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

target_sources(person_detection_int8
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/detection_responder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/image_provider.cpp
  ${CMAKE_CURRENT_LIST_DIR}/person_detection_main_functions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/model_settings.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/person_model_int8/person_detect_model_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/detection_responder.h
  ${CMAKE_CURRENT_LIST_DIR}/src/image_provider.h
  ${CMAKE_CURRENT_LIST_DIR}/person_detection_main_functions.h
  ${CMAKE_CURRENT_LIST_DIR}/src/model_settings.h
  ${CMAKE_CURRENT_LIST_DIR}/src/person_detect_model_data.h
)

target_link_libraries(
  person_detection_int8
  pico-tflmicro
  hardware_pwm
)
cmake_minimum_required(VERSION 3.12)  #set the cmake need version

include(pico_sdk_import.cmake)

project(UCTREONICS_PICO4ML C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

add_definitions(-DSPI0_BAUDRATE=20*1000*1000)
add_definitions(-DSPI1_BAUDRATE=12*1000*1000)

# Add header folder 
include_directories(lcd)
include_directories(fatfs)
include_directories(IMU)
include_directories(Arducam)
include_directories(hm01b0)
include_directories(../../../src/Arducam)


include_directories(microphone)
include_directories(libgwavi)

include_directories(tensorflow/person_detection)
include_directories(tensorflow/micro_speech)
include_directories(tensorflow/magic_wand)

# Add subdirectory
add_subdirectory(lcd)
add_subdirectory(IMU)
add_subdirectory(fatfs)
add_subdirectory(hm01b0)
add_subdirectory(Arducam)
add_subdirectory(microphone)
add_subdirectory(libgwavi)
add_subdirectory(tensorflow)

add_subdirectory(example)
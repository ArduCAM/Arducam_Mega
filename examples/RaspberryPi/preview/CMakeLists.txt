# aux_source_directory(. DIR_CAM_SRCS)
cmake_minimum_required(VERSION 3.18)
project(Preview CXX)
find_package(OpenCV)

include_directories(Arducam)
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(../../../src/Arducam)

add_subdirectory(Arducam)

add_executable(preview main.cpp)
target_link_libraries(preview libarducam  ${OpenCV_LIBS})
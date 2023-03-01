project(ICM42622 C CXX ASM)

aux_source_directory(. ICM42622_SOURCE)

add_library(ICM42622 ${ICM42622_SOURCE})

# target_include_directories(ICM42622 PUBLIC .)

target_link_libraries(ICM42622 PUBLIC hardware_i2c pico_stdlib)


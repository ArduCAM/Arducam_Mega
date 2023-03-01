
project(Libgwavi C CXX ASM)

#Find all surce files and save the name to AVI_SRC parameter
aux_source_directory(. AVI_SRC)
include_directories(../fatfs)
# 生成
add_library(libgwavi ${AVI_SRC})
target_link_libraries(libgwavi PUBLIC fatfs)
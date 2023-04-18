set(srcs        spi.c
                ../../../../../src/Arducam/ArducamCamera.c
                )

set(inc_dirs    ../../../../../src/Arducam
                .)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${inc_dirs})

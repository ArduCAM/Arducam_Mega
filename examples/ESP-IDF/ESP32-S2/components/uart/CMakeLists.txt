set(srcs        uart.c
                ArducamLink.c
                )

set(inc_dirs    ../../../../../src/Arducam
                .)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${inc_dirs} REQUIRES libcamera)

set(srcs        app_main.c)

set(inc_dirs    ../../../../src/Arducam
                ../components/uart
                .)

# set(requires    libcamera)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${inc_dirs} REQUIRES libcamera uart) 
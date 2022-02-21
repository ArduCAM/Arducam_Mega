set(srcs "smartCamera.c"
    "Arducam.c"
    "delay.c"
    "uart.c"
    "spi.c"
    )


idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
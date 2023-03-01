project(Pdm_microphone C CXX ASM)


add_library(pico_pdm_microphone)

target_sources(pico_pdm_microphone PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/pdm_microphone.c
    ${CMAKE_CURRENT_LIST_DIR}/OpenPDM2PCM/OpenPDMFilter.c
)

target_include_directories(pico_pdm_microphone PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_generate_pio_header(pico_pdm_microphone ${CMAKE_CURRENT_LIST_DIR}/pdm_microphone.pio)

target_link_libraries(pico_pdm_microphone PUBLIC pico_stdlib hardware_dma hardware_pio)